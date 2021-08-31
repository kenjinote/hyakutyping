/*!
  \file
  \brief タイピング用の日本語変換

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingTextConverter.h"
#include "RomanConvertTable.h"
#include "KanaConvertTable.h"
#include "UtfString.h"
#include "TypingRecorder.h"
#include "GetTicks.h"

using namespace beego;


struct TypingTextConverter::pImpl {
  const Uint16* convert_table;
  size_t table_length;
  std::vector<Uint16> converted_text;
  TypingRecorder* recorder;
  size_t start_ticks;
  ConvertType type_;

  pImpl(const Uint16* convertTable, size_t tableLength)
    : convert_table(convertTable), table_length(tableLength), recorder(NULL),
      start_ticks(0) {
  }

  void replaceMatched(size_t text_index, size_t match_index,
                      size_t text_left, size_t match_length) {

    size_t replace_length =
      ustrlen(&convert_table[match_index + table_length]);

    // 変換するとテキストが長くなる場合の調整
    int extend_length = static_cast<int>(replace_length - text_left);
    if (extend_length > 0) {
      // converted_text を伸長する
      converted_text.resize(converted_text.size() + extend_length, 0x0);
    }

    for (size_t i = 0; i < replace_length; ++i) {
      size_t replace_index = match_index + table_length + i;
      converted_text[text_index + i] = convert_table[replace_index];
    }

    // 変換してテキストが短くなった場合の調整
    int replaced_diff = static_cast<int>(match_length - replace_length);
    if (replaced_diff > 0) {
      std::vector<Uint16>::iterator first =
        converted_text.begin() + text_index + (match_length - replaced_diff);
      converted_text.erase(first, first + replaced_diff);
    }
  }

  int typstrncmp(const Uint16* a, const Uint16* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
      if ((a[i] == 0x0) || (b[i] == 0x0)) {
        return a[i] - b[i];
      }

      int cmp = (type_ == Roman) ?
          (tolower(a[i]) - tolower(b[i])) : (a[i] - b[i]);
      if (cmp != 0) {
        return cmp;
      }
    }
    return 0;
  }

  void convert(void) {

    // 変換候補と入力文字列との比較を行う
    for (size_t text_index = 0;
         text_index < converted_text.size(); ++text_index) {

      size_t text_length = converted_text.size();
      for (size_t match_index = 0, first = 0; convert_table[first] != 0x0;
           match_index += 2 * table_length, first += 2 * table_length) {
        size_t match_length = ustrlen(&convert_table[first]);

        // !!! 文字列長さ的にマッチしない場合は、比較しないようにする
        // !!! とりあえず、0x0 を終端に配置してごまかす
        converted_text.push_back(0x0);
        int cmp = typstrncmp(&convert_table[first],
                             &converted_text[text_index], match_length);
        converted_text.pop_back();
        if (! cmp) {
          size_t text_left = text_length - text_index;
          replaceMatched(text_index, match_index, text_left, match_length);
          break;
        }
      }
    }
  }

  void recordTyping(char type) {
    size_t ticks = GetTicks() - start_ticks;
    if (recorder) {
      recorder->recordTyping(type, ticks);
    }
  }
};


TypingTextConverter::TypingTextConverter(ConvertType type)
  : pimpl((type == Roman) ?
          new pImpl(RomanTable[0][0], ROMAN_CONVERT_SIZE_MAX) :
          new pImpl(KanaTable[0][0], KANA_CONVERT_SIZE_MAX)) {
  pimpl->type_ = type;
}


TypingTextConverter::~TypingTextConverter(void) {
}


void TypingTextConverter::clear(void) {

  pimpl->recordTyping(TypingRecorder::Clear);

  pimpl->converted_text.clear();
}


void TypingTextConverter::setConvertBuffer(const std::vector<Uint16>& text) {
  pimpl->converted_text = text;
}


void TypingTextConverter::getConvertBuffer(std::vector<Uint16>& buffer) {
  buffer = pimpl->converted_text;
}


bool TypingTextConverter::addChar(char ch) {

  if (ch == ' ') {
    // スペース入力は、処理しない
    return false;
  }

  pimpl->recordTyping(ch);

  pimpl->converted_text.push_back(ch);
  pimpl->convert();
  return true;
}


bool TypingTextConverter::moveLeft(void) {
  // !!! 後回し
  return false;
}


bool TypingTextConverter::moveRight(void) {
  // !!! 後回し
  return false;
}


bool TypingTextConverter::moveUp(void) {
  // 実装しない
  return false;
}


bool TypingTextConverter::moveDown(void) {
  // 実装しない
  return false;
}


bool TypingTextConverter::escapePressed(void) {

  pimpl->recordTyping(TypingRecorder::Clear);

  pimpl->converted_text.clear();
  return true;
}


bool TypingTextConverter::deleteBack(void) {

  pimpl->recordTyping(TypingRecorder::BackSpace);

  if (! pimpl->converted_text.empty()) {
    pimpl->converted_text.pop_back();
    return true;
  }
  return false;
}


bool TypingTextConverter::deleteCurrent(void) {
  // !!! 後回し...、にする程でもないかも
  // !!! いや、カーソルの概念は後回しにしましょう
  return false;
}


bool TypingTextConverter::convertInput(void) {
  // 実装しない
  return false;
}


void TypingTextConverter::registerRecorder(TypingRecorder* recorder) {
  pimpl->recorder = recorder;
}


void TypingTextConverter::registerStartTicks(size_t start_ticks) {
  pimpl->start_ticks = start_ticks;
}


TypingTextConverter::ConvertType TypingTextConverter::getConvertType(void) {
  return pimpl->type_;
}
