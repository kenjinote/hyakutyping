/*!
  \file
  \brief �^�C�s���O�p�̓��{��ϊ�

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

    // �ϊ�����ƃe�L�X�g�������Ȃ�ꍇ�̒���
    int extend_length = static_cast<int>(replace_length - text_left);
    if (extend_length > 0) {
      // converted_text ��L������
      converted_text.resize(converted_text.size() + extend_length, 0x0);
    }

    for (size_t i = 0; i < replace_length; ++i) {
      size_t replace_index = match_index + table_length + i;
      converted_text[text_index + i] = convert_table[replace_index];
    }

    // �ϊ����ăe�L�X�g���Z���Ȃ����ꍇ�̒���
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

    // �ϊ����Ɠ��͕�����Ƃ̔�r���s��
    for (size_t text_index = 0;
         text_index < converted_text.size(); ++text_index) {

      size_t text_length = converted_text.size();
      for (size_t match_index = 0, first = 0; convert_table[first] != 0x0;
           match_index += 2 * table_length, first += 2 * table_length) {
        size_t match_length = ustrlen(&convert_table[first]);

        // !!! �����񒷂��I�Ƀ}�b�`���Ȃ��ꍇ�́A��r���Ȃ��悤�ɂ���
        // !!! �Ƃ肠�����A0x0 ���I�[�ɔz�u���Ă��܂���
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
    // �X�y�[�X���͂́A�������Ȃ�
    return false;
  }

  pimpl->recordTyping(ch);

  pimpl->converted_text.push_back(ch);
  pimpl->convert();
  return true;
}


bool TypingTextConverter::moveLeft(void) {
  // !!! ���
  return false;
}


bool TypingTextConverter::moveRight(void) {
  // !!! ���
  return false;
}


bool TypingTextConverter::moveUp(void) {
  // �������Ȃ�
  return false;
}


bool TypingTextConverter::moveDown(void) {
  // �������Ȃ�
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
  // !!! ���...�A�ɂ�����ł��Ȃ�����
  // !!! ����A�J�[�\���̊T�O�͌�񂵂ɂ��܂��傤
  return false;
}


bool TypingTextConverter::convertInput(void) {
  // �������Ȃ�
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
