/*!
  \file
  \brief �^�C�s���O���ʂ̕\��

  \author Satofumi KAMIMURA

  $Id$

  \todo ���ʂ��t�F�[�h���Ȃ���\������
  \todo �d�����Ȃ�Ƃ�����B�Ђǂ�...
  \todo ���^�C�v���̐��l��K�؂ɂ���
*/

#include "ResultDrawer.h"
#include "ResultDrawer_uni.h"
#include "DrawsDefinition.h"
#include "SystemDefinition.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "AccessRecordPC.h"
#include "AccessRecordWeb.h"
#include "GuiManager.h"
#include "Layer.h"
#include "InputHandler.h"
#include "InputEvent.h"
#include "InputReceiveComponent.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "TextFadeSurface.h"
#include "LabelComponent.h"
#include "SdlUtils.h"
#include "Delay.h"
#include "UtfString.h"

using namespace beego;


struct ResultDrawer::pImpl {
  TypingMode typing_mode_;
  CommonResources* common_;
  bool is_decided_;
  InputEvent input_event_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  std::vector<Surface> name_surfaces_;
  std::vector<Component> name_labels_;
  std::vector<Surface> num_surfaces_;
  std::vector<Component> num_labels_;
  std::vector<Surface> unit_surfaces_;
  std::vector<Component> unit_labels_;

  pImpl(TypingMode mode)
    : typing_mode_(mode), common_(CommonResources::getObject()),
      is_decided_(false),
      title_property_(common_->font_, ResultDrawer_title,
                      WakaDrawSize, Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)) {
  }

  ~pImpl(void) {
    clearComponents();
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(title_label_);
    for (std::vector<Component>::iterator it = name_labels_.begin();
         it != name_labels_.end(); ++it) {
      common_->front_layer_->push_front(*it);
    }
    for (std::vector<Component>::iterator it = num_labels_.begin();
         it != num_labels_.end(); ++it) {
      common_->front_layer_->push_front(*it);
    }
    for (std::vector<Component>::iterator it = unit_labels_.begin();
         it != unit_labels_.end(); ++it) {
      common_->front_layer_->push_front(*it);
    }

    is_decided_ = false;
    common_->input_receiver_->clear();
    common_->front_layer_->push_front(common_->user_label_);
  }

  void removeComponents(void) {
    common_->front_layer_->remove(title_label_);
    for (std::vector<Component>::iterator it = name_labels_.begin();
         it != name_labels_.end(); ++it) {
      common_->front_layer_->remove(*it);
    }
    for (std::vector<Component>::iterator it = num_labels_.begin();
         it != num_labels_.end(); ++it) {
      common_->front_layer_->remove(*it);
    }
    for (std::vector<Component>::iterator it = unit_labels_.begin();
         it != unit_labels_.end(); ++it) {
      common_->front_layer_->remove(*it);
    }

    common_->input_receiver_->clear();
    common_->front_layer_->remove(common_->user_label_);
  }

  void clearComponents(void) {
    name_surfaces_.clear();
    name_labels_.clear();
    num_surfaces_.clear();
    num_labels_.clear();
    unit_surfaces_.clear();
    unit_labels_.clear();
  }

  void updateComponent(void) {

    SDL_Rect position;
    set_SdlRect(&position, centerPosition(title_label_, 640/2), 480/6);
    title_label_->setPosition(&position);

    // �a�̐�
    TextProperty text_property(common_->font_, ResultDrawer_num,
                               MenuSize, Fore, Back, true);
    name_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    name_labels_.push_back
      (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

    // ��
    char buffer[13];
    int waka_num = static_cast<int>(common_->recorder_->getWakaNum());
    sprintf(buffer, "%d", waka_num);
    std::vector<Uint16> num_text;
    ustrcat(num_text, buffer);
    text_property.utext = &num_text[0];
    num_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    num_labels_.push_back
      (static_cast<Component>(new LabelComponent(num_surfaces_.back())));
    text_property.utext = ResultDrawer_syu;
    text_property.size = NormalSize;
    unit_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    unit_labels_.push_back
      (static_cast<Component>(new LabelComponent(unit_surfaces_.back())));

    if (typing_mode_ == ShimonokuTyping) {
      // �^�C�v���x
      text_property.utext = ResultDrawer_types;
      text_property.size = MenuSize;
      name_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      name_labels_.push_back
        (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

      // ��/�b
      num_text.clear();
      double typing_speed = common_->recorder_->getTypingSpeed();
      sprintf(buffer, "%.1f", typing_speed);
      ustrcat(num_text, buffer);
      text_property.utext = &num_text[0];
      num_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      num_labels_.push_back
        (static_cast<Component>(new LabelComponent(num_surfaces_.back())));
      text_property.utext = ResultDrawer_vel;
      text_property.size = NormalSize;
      unit_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      unit_labels_.push_back
        (static_cast<Component>(new LabelComponent(unit_surfaces_.back())));
    }

    // ���͊J�n�܂ł̎���
    text_property.utext = ResultDrawer_delay;
    text_property.size = MenuSize;
    name_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    name_labels_.push_back
      (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

    // �b
    num_text.clear();
    double delay_sec = common_->recorder_->getStartDelay();
    sprintf(buffer, "%.2f", delay_sec);
    ustrcat(num_text, buffer);
    text_property.utext = &num_text[0];
    num_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    num_labels_.push_back
      (static_cast<Component>(new LabelComponent(num_surfaces_.back())));
    text_property.utext = ResultDrawer_sec;
    text_property.size = NormalSize;
    unit_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    unit_labels_.push_back
      (static_cast<Component>(new LabelComponent(unit_surfaces_.back())));

    if (typing_mode_ == ShimonokuTyping) {
      // �����^�C�v��
      text_property.utext = ResultDrawer_miss;
      text_property.size = MenuSize;
      name_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      name_labels_.push_back
        (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

      // ��
      num_text.clear();
      int miss_types = static_cast<int>(common_->recorder_->getMissTypes());
      sprintf(buffer, "%d", miss_types);
      ustrcat(num_text, buffer);
      text_property.utext = &num_text[0];
      num_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      num_labels_.push_back
        (static_cast<Component>(new LabelComponent(num_surfaces_.back())));
      text_property.utext = ResultDrawer_times;
      text_property.size = NormalSize;
      unit_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      unit_labels_.push_back
        (static_cast<Component>(new LabelComponent(unit_surfaces_.back())));
    }

    // �ĉ��i�̍��ڈʒu����ɂ���
    size_t unit_max_length = unit_labels_.back()->getWidth();

    // �R���|�[�l���g�̈ʒu�v�Z
    position.y += ResultOffset - 10;
    SDL_Rect num_position;
    num_position.y = position.y;
    SDL_Rect unit_position;
    unit_position.y = position.y + (MenuSize - NormalSize);

    position.x = (640 - ResultWidth)/2;
    size_t num_pos_right = 640 - (640 - ResultWidth)/2 - unit_max_length;
    unit_position.x = static_cast<int>(num_pos_right);

    // ���ڂ̔z�u
    for (std::vector<Component>::iterator it = name_labels_.begin();
         it != name_labels_.end(); ++it) {
      position.y += ResultOffset;
      (*it)->setPosition(&position);
    }

    // ���l�̔z�u
    for (std::vector<Component>::iterator it = num_labels_.begin();
         it != num_labels_.end(); ++it) {
      num_position.y += ResultOffset;
      num_position.x = rightPosition(*it, static_cast<int>(num_pos_right));
      (*it)->setPosition(&num_position);
    }

    // �P�ʂ̔z�u
    for (std::vector<Component>::iterator it = unit_labels_.begin();
         it != unit_labels_.end(); ++it) {
      unit_position.y += ResultOffset;
      (*it)->setPosition(&unit_position);
    }
  }
};


ResultDrawer::ResultDrawer(TypingMode mode) : pimpl(new pImpl(mode)) {
}


ResultDrawer::~ResultDrawer(void) {
}


void ResultDrawer::createResult(void) {

  // �\�����錋�ʂ̌v�Z
  const char* user_name = pimpl->common_->getUserName();
  pimpl->common_->recorder_->evaluate(user_name);

  // ���[�J���t�@�C���ւ̃f�[�^�L�^
  AccessRecordPC pc_record(pimpl->typing_mode_);
  TypingRecorder::GameSettings&
    record_data = pimpl->common_->recorder_->getRecordData();
  pc_record.save(record_data);

  // Web �ւ̃f�[�^�o�^
  if (pimpl->common_->isSendData() && (pimpl->common_->getUserIndex() > 0)) {
    // !!! �Ƃ肠�����A5��̂Ƃ��݂̂Ƃ���B�f�[�^�ʂƂ��C�ɂȂ邵��
    if (record_data.waka_num <= 5) {
      AccessRecordWeb web_record;
      web_record.save(record_data);
    }
  }

  // �R���|�[�l���g�̐���
  pimpl->clearComponents();
  pimpl->updateComponent();

  // �R���|�[�l���g�̔z�u
  // !!! �����Ŕz�u���Ăяo���̂́A�Ȃ񂩕ςȋC�����邪�A�܂��A���̂���
  pimpl->placeComponents();

  // ���y�̒�~��҂�
  pimpl->common_->stopMusic(3000);
}


void ResultDrawer::drawResult(size_t ticks) {
  static_cast<void>(ticks);

  // !!!
  // !!! ticks �ɏ]���āA�`����s��
}


void ResultDrawer::placeComponents(void) {
  pimpl->placeComponents();
}


void ResultDrawer::removeComponents(void) {
  pimpl->removeComponents();

  // ���y�̒�~
  pimpl->common_->haltMusic();
}


bool ResultDrawer::keyPressed(void) {

  pimpl->common_->input_receiver_->updateInputEvent(pimpl->input_event_);
  pimpl->is_decided_ |=
    InputEvent::isReleased(pimpl->input_event_, SDLK_RETURN);

  return pimpl->is_decided_;
}
