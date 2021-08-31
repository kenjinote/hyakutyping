/*!
  \file
  \brief 下の句の入力処理

  \author Satofumi KAMIMURA

  $Id$

  \todo カーソルが表示されないのを、なんとかする
  \todo タイプ音を導入する
  \todo ミス音を大きくする
*/

#include "ShimonokuInput.h"
#include "DrawsDefinition.h"
#include "PracticeNum_uni.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "AccessSettings.h"
#include "WakaDrawer.h"
#include "WakaData.h"
#include "TypingRecorder.h"
#include "GuiManager.h"
#include "Layer.h"
#include "TextInputComponent.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "LabelComponent.h"
#include "SdlUtils.h"
#include "GetTicks.h"
#include "UtfString.h"
#include "RomanCreator.h"

using namespace beego;


struct ShimonokuInput::pImpl {
  TypingMode typing_mode_;
  TypingTextConverter::ConvertType type_;
  CommonResources* common_;
  WakaDrawer waka_drawer_;
  int input_complete_;
  TextProperty text_property_;
  boost::shared_ptr<TextInputComponent> text_input_;
  TypingTextConverter converter_;
  size_t start_ticks_;
  size_t waka_index_;
  size_t waka_no_;
  std::vector<std::vector<Uint16> > judge_text_;
  RomanCreator roman_creator_;
  std::vector<Uint16> roman_complete_sample_[2];

  Component left_label_;

  pImpl(TypingMode mode, TypingTextConverter::ConvertType type)
    : typing_mode_(mode), type_(type), common_(CommonResources::getObject()),
      waka_drawer_(typing_mode_), input_complete_(0),
      text_property_(common_->font_, "", WakaDrawSize, White, Black, false),
      text_input_(new TextInputComponent(InputWidth, InputSize, text_property_,
                                         TextOffset,
                                         (InputSize - WakaDrawSize) / 2)),
      converter_(type_), start_ticks_(0), waka_index_(0), waka_no_(0) {

    // !!! ここ以前に、recorder の変換モードを作り直しておく必要がある
    // !!! このあたりで、TypingTextConverter の変換モードを作り直す、か？
    // !!! または、オプションで変更したときに作り直すべきか？
    converter_.registerRecorder(common_->recorder_);

    // !!! いずれ、waka_index を登録するメソッドを持つ、のか？
    // !!! または、ソートされたキーを持つ配列を受け渡す感じになるのかな？
    // !!! まぁ、実装するときに考えましょう

    SDL_Rect position;
    set_SdlRect(&position, (640 - InputWidth)/2,
                bottomPosition(text_input_, 480) - BottomOffset);
    text_input_->setPosition(&position);
    text_input_->swapTextConverter(&converter_);

    // 警告に対処
    //if (PracticeNum_5 || PracticeNum_10 ||
    //PracticeNum_50 || PracticeNum_100 ||
    //YomibitoName || KanjiWaka);
  }

  void clearInputState(void) {
    text_input_->clear();
    text_input_->releaseDecided();
  }

  void createJudgeText(void) {
    judge_text_.resize(5);
    for (size_t i = 0; i < 5; ++i) {
      judge_text_[i].clear();
      size_t n = ustrlen(KanaWaka[waka_no_][i]);

      for (size_t j = 0; j < n; ++j) {
        Uint16 uch = KanaWaka[waka_no_][i][j];
        if (uch == 0x3090) {
          // ゐ: 0x3090 -> 0x3044
          uch = 0x3044;
        } else if (uch == 0x3091) {
          // ゑ: 0x3091 -> 0x3048
          uch = 0x3048;
        }
        judge_text_[i].push_back(uch);
      }
      judge_text_[i].push_back(0x0);
    }

    // ローマ字入力候補の作成
    for (int i = 0; i < 2; ++i) {
      std::vector<Uint16> roman_sample = judge_text_[i + 3];
      if (typing_mode_ == KimarijiTyping) {
        // 下の句入力のときは、２文字のみの入力候補にする
        roman_sample[2] = 0x0;
      }

      if (type_ == TypingTextConverter::Kana) {
        // "かな入力" のとき。ローマ字に変換しない
        roman_complete_sample_[i] = roman_sample;
      } else {
        // "ローマ字入力" のとき
        roman_creator_.convert(roman_complete_sample_[i], &roman_sample[0]);
      }
    }
  }

  bool createRomanSample(std::vector<Uint16>& roman_sample) {

    // 入力された文字列をローマ字変換する
    std::vector<Uint16> inputed_text;
    std::vector<Uint16> inputed_roman;
    if (text_input_->getBuffer(inputed_text) > 0) {
      if (type_ == TypingTextConverter::Kana) {
        // "かな入力" のとき。変換しない文字列を inputed_roman として扱う
        inputed_roman = inputed_text;

      } else {
        // "ローマ字入力" のとき
        RomanCreator roman_creator;
        roman_creator.convert(inputed_roman, &inputed_text[0]);
      }
    }

    // 文字列の共通部分を取り除いた文字列を返す
    // !!! とりあえず、べたで作る
    size_t complete_n = roman_complete_sample_[input_complete_].size() - 1;
    size_t input_n = inputed_roman.size();
    size_t n = (complete_n > input_n) ? input_n : complete_n;
    size_t i = 0;
    for (; i < n; ++i) {
      if (inputed_roman[i] != roman_complete_sample_[input_complete_][i]) {
        break;
      }
    }
    ustrcat(roman_sample, &roman_complete_sample_[input_complete_][i]);
#if 0
    // !!! デバッグ表示
    for (std::vector<Uint16>::iterator it = roman_sample.begin();
         it != roman_sample.end(); ++it) {
      fprintf(stderr, "%x", *it);
      fprintf(stderr, "[%x], ", roman_complete_sample[input_complete][i]);
    }
    fprintf(stderr, "\n");
#endif

    bool need_bs = true;
    if (((i == 0) && (input_n == 0)) ||
        ((i == complete_n) && (i == input_n)) ||
        ((i + 1) == input_n)) {
      need_bs = false;
    }
    return need_bs;
  }
};


ShimonokuInput::ShimonokuInput(TypingMode mode,
                               TypingTextConverter::ConvertType type)
  : pimpl(new pImpl(mode, type)) {
}


ShimonokuInput::~ShimonokuInput(void) {
}


void ShimonokuInput::setNextWaka(void) {
  ++pimpl->waka_index_;
  if (pimpl->waka_index_ >= 100) {
    pimpl->waka_index_ = 0;
  }
}


size_t ShimonokuInput::getCurrentWakaNo(void) {
  return pimpl->common_->waka_order_[pimpl->waka_index_];
}


void ShimonokuInput::updateWaka(void) {

  pimpl->waka_no_ = getCurrentWakaNo();

  // 和歌の番号を記録
  pimpl->common_->recorder_->recordWaka(pimpl->waka_no_);
  size_t ticks = GetTicks();
  pimpl->start_ticks_ = ticks;
  pimpl->converter_.registerStartTicks(ticks);

  // 入力コンポーネントの初期化
  pimpl->clearInputState();
  pimpl->input_complete_ = 0;

  // 現代仮名使い判定用の文字列の作成
  pimpl->createJudgeText();

  // コンポーネントの描画
  pimpl->waka_drawer_.setWakaIndex(pimpl->waka_no_);

  // ローマ字入力候補の作成
  std::vector<Uint16> roman_sample;

  // "かな入力" のときには、ここで変換前の文字列を渡す
  // !!!
  pimpl->createRomanSample(roman_sample);
  pimpl->waka_drawer_.setInputSample(pimpl->judge_text_, roman_sample);

  // コンポーネントの配置
  placeComponents();
}


void ShimonokuInput::drawWaka(void) {

  // 上の句、下の句の更新
  pimpl->waka_drawer_.draw();
}


void ShimonokuInput::drawInput(void) {

  if (! pimpl->text_input_->isChanged()) {
    return;
  }

  // ローマ字入力候補の作成
  std::vector<Uint16> roman_sample;
  bool need_bs = pimpl->createRomanSample(roman_sample);

  bool is_erased = pimpl->text_input_->isErased();
  if ((! is_erased) && (need_bs != false)) {
    // ミス音の再生。need_bs の表示中の入力で、ミス音を鳴らす。削除キーは例外
    pimpl->common_->playEffect(Miss);

  } else if ((! is_erased) && (! pimpl->text_input_->empty())) {
    // タイプ音の再生。削除キー以外の場合に、効果音を鳴らす
    pimpl->common_->playEffect(Type);
  }

  // 入力候補の更新
  pimpl->waka_drawer_.updateInputSample(pimpl->input_complete_, roman_sample,
                                        need_bs);

  // 下の句タイピングのときは、入力が確定されていなければ、判定しない
  if (pimpl->typing_mode_ == ShimonokuTyping) {
    if (! pimpl->text_input_->isDecided()) {
      return;
    }
    size_t ticks = GetTicks() - pimpl->start_ticks_;
    pimpl->common_->recorder_->recordTyping(TypingRecorder::Return, ticks);
  }
  pimpl->text_input_->releaseDecided();

  // 入力文字列の判定
  std::vector<Uint16> buffer;
  size_t inputed_length = pimpl->text_input_->getBuffer(buffer);
  size_t ku_index = 3 + pimpl->input_complete_;
  size_t n = ustrlen(KanaWaka[pimpl->waka_no_][ku_index]);

  switch (pimpl->typing_mode_) {
  case ShimonokuTyping:
    // 入力が完了したかの判定
    if ((inputed_length > 0) && (n == inputed_length) &&
        (! ustrncmp(&pimpl->judge_text_[ku_index][0], &buffer[0], n))) {

      size_t ticks = GetTicks() - pimpl->start_ticks_;
      pimpl->common_->recorder_->recordTyping(TypingRecorder::Complete, ticks);

      ++pimpl->input_complete_;
      pimpl->clearInputState();
    }
    break;

  case KimarijiTyping:
    // 最初の２文字で入力判定を行う
    if ((inputed_length >= 2) &&
        (! ustrncmp(&pimpl->judge_text_[ku_index][0], &buffer[0], 2))) {
      ++pimpl->input_complete_;
      pimpl->clearInputState();
    }
    break;
  }
}


bool ShimonokuInput::isComplete(void) {

  switch (pimpl->typing_mode_) {
  case ShimonokuTyping:
    return (pimpl->input_complete_ >= 2) ? true : false;
    break;

  case KimarijiTyping:
    return (pimpl->input_complete_ >= 1) ? true : false;
    break;

  default:
    return false;
  }
}


void ShimonokuInput::placeComponents(void) {
  pimpl->common_->front_layer_->push_front(pimpl->text_input_);
}


void ShimonokuInput::removeComponents(void) {
  removeInputComponents();
  pimpl->waka_drawer_.removeComponents();
}


void ShimonokuInput::removeInputComponents(void) {
  pimpl->waka_drawer_.removeInputComponents();

  pimpl->text_input_->clear();
  pimpl->common_->front_layer_->remove(pimpl->text_input_);
}


bool ShimonokuInput::textEmpty(void) {
  return pimpl->text_input_->empty();
}


void ShimonokuInput::placeLeftNum(size_t left_num) {

  std::vector<Uint16> utext;
  ustrcat(utext, PracticeNum_left);
  char buffer[13];
  sprintf(buffer, "%d", left_num);
  ustrcat(utext, buffer);
  ustrcat(utext, PracticeNum_syu);

  TextProperty left_property(pimpl->common_->font_,
                             (left_num > 0) ?  &utext[0] : PracticeNum_complete,
                             NormalSize, Fore, Back, true);
  Surface left_surface(new TextSurface(left_property));
  Component new_left_label(new LabelComponent(left_surface));
  std::swap(pimpl->left_label_, new_left_label);

  SDL_Rect position;
  set_SdlRect(&position, TextOffset,
              bottomPosition(pimpl->left_label_, 480) - TextOffset);
  pimpl->left_label_->setPosition(&position);
  pimpl->common_->front_layer_->push_front(pimpl->left_label_);
}


void ShimonokuInput::removeLeftNum(void) {
  pimpl->common_->front_layer_->remove(pimpl->left_label_);
}
