/*!
  \file
  \brief ���̋�̓��͏���

  \author Satofumi KAMIMURA

  $Id$

  \todo �J�[�\�����\������Ȃ��̂��A�Ȃ�Ƃ�����
  \todo �^�C�v���𓱓�����
  \todo �~�X����傫������
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

    // !!! �����ȑO�ɁArecorder �̕ϊ����[�h����蒼���Ă����K�v������
    // !!! ���̂�����ŁATypingTextConverter �̕ϊ����[�h����蒼���A���H
    // !!! �܂��́A�I�v�V�����ŕύX�����Ƃ��ɍ�蒼���ׂ����H
    converter_.registerRecorder(common_->recorder_);

    // !!! ������Awaka_index ��o�^���郁�\�b�h�����A�̂��H
    // !!! �܂��́A�\�[�g���ꂽ�L�[�����z����󂯓n�������ɂȂ�̂��ȁH
    // !!! �܂��A��������Ƃ��ɍl���܂��傤

    SDL_Rect position;
    set_SdlRect(&position, (640 - InputWidth)/2,
                bottomPosition(text_input_, 480) - BottomOffset);
    text_input_->setPosition(&position);
    text_input_->swapTextConverter(&converter_);

    // �x���ɑΏ�
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
          // ��: 0x3090 -> 0x3044
          uch = 0x3044;
        } else if (uch == 0x3091) {
          // ��: 0x3091 -> 0x3048
          uch = 0x3048;
        }
        judge_text_[i].push_back(uch);
      }
      judge_text_[i].push_back(0x0);
    }

    // ���[�}�����͌��̍쐬
    for (int i = 0; i < 2; ++i) {
      std::vector<Uint16> roman_sample = judge_text_[i + 3];
      if (typing_mode_ == KimarijiTyping) {
        // ���̋���͂̂Ƃ��́A�Q�����݂̂̓��͌��ɂ���
        roman_sample[2] = 0x0;
      }

      if (type_ == TypingTextConverter::Kana) {
        // "���ȓ���" �̂Ƃ��B���[�}���ɕϊ����Ȃ�
        roman_complete_sample_[i] = roman_sample;
      } else {
        // "���[�}������" �̂Ƃ�
        roman_creator_.convert(roman_complete_sample_[i], &roman_sample[0]);
      }
    }
  }

  bool createRomanSample(std::vector<Uint16>& roman_sample) {

    // ���͂��ꂽ����������[�}���ϊ�����
    std::vector<Uint16> inputed_text;
    std::vector<Uint16> inputed_roman;
    if (text_input_->getBuffer(inputed_text) > 0) {
      if (type_ == TypingTextConverter::Kana) {
        // "���ȓ���" �̂Ƃ��B�ϊ����Ȃ�������� inputed_roman �Ƃ��Ĉ���
        inputed_roman = inputed_text;

      } else {
        // "���[�}������" �̂Ƃ�
        RomanCreator roman_creator;
        roman_creator.convert(inputed_roman, &inputed_text[0]);
      }
    }

    // ������̋��ʕ�������菜�����������Ԃ�
    // !!! �Ƃ肠�����A�ׂ��ō��
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
    // !!! �f�o�b�O�\��
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

  // �a�̂̔ԍ����L�^
  pimpl->common_->recorder_->recordWaka(pimpl->waka_no_);
  size_t ticks = GetTicks();
  pimpl->start_ticks_ = ticks;
  pimpl->converter_.registerStartTicks(ticks);

  // ���̓R���|�[�l���g�̏�����
  pimpl->clearInputState();
  pimpl->input_complete_ = 0;

  // ���㉼���g������p�̕�����̍쐬
  pimpl->createJudgeText();

  // �R���|�[�l���g�̕`��
  pimpl->waka_drawer_.setWakaIndex(pimpl->waka_no_);

  // ���[�}�����͌��̍쐬
  std::vector<Uint16> roman_sample;

  // "���ȓ���" �̂Ƃ��ɂ́A�����ŕϊ��O�̕������n��
  // !!!
  pimpl->createRomanSample(roman_sample);
  pimpl->waka_drawer_.setInputSample(pimpl->judge_text_, roman_sample);

  // �R���|�[�l���g�̔z�u
  placeComponents();
}


void ShimonokuInput::drawWaka(void) {

  // ��̋�A���̋�̍X�V
  pimpl->waka_drawer_.draw();
}


void ShimonokuInput::drawInput(void) {

  if (! pimpl->text_input_->isChanged()) {
    return;
  }

  // ���[�}�����͌��̍쐬
  std::vector<Uint16> roman_sample;
  bool need_bs = pimpl->createRomanSample(roman_sample);

  bool is_erased = pimpl->text_input_->isErased();
  if ((! is_erased) && (need_bs != false)) {
    // �~�X���̍Đ��Bneed_bs �̕\�����̓��͂ŁA�~�X����炷�B�폜�L�[�͗�O
    pimpl->common_->playEffect(Miss);

  } else if ((! is_erased) && (! pimpl->text_input_->empty())) {
    // �^�C�v���̍Đ��B�폜�L�[�ȊO�̏ꍇ�ɁA���ʉ���炷
    pimpl->common_->playEffect(Type);
  }

  // ���͌��̍X�V
  pimpl->waka_drawer_.updateInputSample(pimpl->input_complete_, roman_sample,
                                        need_bs);

  // ���̋�^�C�s���O�̂Ƃ��́A���͂��m�肳��Ă��Ȃ���΁A���肵�Ȃ�
  if (pimpl->typing_mode_ == ShimonokuTyping) {
    if (! pimpl->text_input_->isDecided()) {
      return;
    }
    size_t ticks = GetTicks() - pimpl->start_ticks_;
    pimpl->common_->recorder_->recordTyping(TypingRecorder::Return, ticks);
  }
  pimpl->text_input_->releaseDecided();

  // ���͕�����̔���
  std::vector<Uint16> buffer;
  size_t inputed_length = pimpl->text_input_->getBuffer(buffer);
  size_t ku_index = 3 + pimpl->input_complete_;
  size_t n = ustrlen(KanaWaka[pimpl->waka_no_][ku_index]);

  switch (pimpl->typing_mode_) {
  case ShimonokuTyping:
    // ���͂������������̔���
    if ((inputed_length > 0) && (n == inputed_length) &&
        (! ustrncmp(&pimpl->judge_text_[ku_index][0], &buffer[0], n))) {

      size_t ticks = GetTicks() - pimpl->start_ticks_;
      pimpl->common_->recorder_->recordTyping(TypingRecorder::Complete, ticks);

      ++pimpl->input_complete_;
      pimpl->clearInputState();
    }
    break;

  case KimarijiTyping:
    // �ŏ��̂Q�����œ��͔�����s��
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
