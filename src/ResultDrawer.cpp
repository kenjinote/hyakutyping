/*!
  \file
  \brief タイピング結果の表示

  \author Satofumi KAMIMURA

  $Id$

  \todo 結果をフェードしながら表示する
  \todo 重複をなんとかする。ひどい...
  \todo 総タイプ数の数値を適切にする
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

    // 和歌数
    TextProperty text_property(common_->font_, ResultDrawer_num,
                               MenuSize, Fore, Back, true);
    name_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    name_labels_.push_back
      (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

    // 首
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
      // タイプ速度
      text_property.utext = ResultDrawer_types;
      text_property.size = MenuSize;
      name_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      name_labels_.push_back
        (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

      // 回/秒
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

    // 入力開始までの時間
    text_property.utext = ResultDrawer_delay;
    text_property.size = MenuSize;
    name_surfaces_.push_back
      (static_cast<Surface>(new TextFadeSurface(text_property)));
    name_labels_.push_back
      (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

    // 秒
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
      // 訂正タイプ数
      text_property.utext = ResultDrawer_miss;
      text_property.size = MenuSize;
      name_surfaces_.push_back
        (static_cast<Surface>(new TextFadeSurface(text_property)));
      name_labels_.push_back
        (static_cast<Component>(new LabelComponent(name_surfaces_.back())));

      // 回
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

    // 再下段の項目位置を基準にする
    size_t unit_max_length = unit_labels_.back()->getWidth();

    // コンポーネントの位置計算
    position.y += ResultOffset - 10;
    SDL_Rect num_position;
    num_position.y = position.y;
    SDL_Rect unit_position;
    unit_position.y = position.y + (MenuSize - NormalSize);

    position.x = (640 - ResultWidth)/2;
    size_t num_pos_right = 640 - (640 - ResultWidth)/2 - unit_max_length;
    unit_position.x = static_cast<int>(num_pos_right);

    // 項目の配置
    for (std::vector<Component>::iterator it = name_labels_.begin();
         it != name_labels_.end(); ++it) {
      position.y += ResultOffset;
      (*it)->setPosition(&position);
    }

    // 数値の配置
    for (std::vector<Component>::iterator it = num_labels_.begin();
         it != num_labels_.end(); ++it) {
      num_position.y += ResultOffset;
      num_position.x = rightPosition(*it, static_cast<int>(num_pos_right));
      (*it)->setPosition(&num_position);
    }

    // 単位の配置
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

  // 表示する結果の計算
  const char* user_name = pimpl->common_->getUserName();
  pimpl->common_->recorder_->evaluate(user_name);

  // ローカルファイルへのデータ記録
  AccessRecordPC pc_record(pimpl->typing_mode_);
  TypingRecorder::GameSettings&
    record_data = pimpl->common_->recorder_->getRecordData();
  pc_record.save(record_data);

  // Web へのデータ登録
  if (pimpl->common_->isSendData() && (pimpl->common_->getUserIndex() > 0)) {
    // !!! とりあえず、5首のときのみとする。データ量とか気になるしな
    if (record_data.waka_num <= 5) {
      AccessRecordWeb web_record;
      web_record.save(record_data);
    }
  }

  // コンポーネントの生成
  pimpl->clearComponents();
  pimpl->updateComponent();

  // コンポーネントの配置
  // !!! ここで配置を呼び出すのは、なんか変な気もするが、まぁ、そのうち
  pimpl->placeComponents();

  // 音楽の停止を待つ
  pimpl->common_->stopMusic(3000);
}


void ResultDrawer::drawResult(size_t ticks) {
  static_cast<void>(ticks);

  // !!!
  // !!! ticks に従って、描画を行う
}


void ResultDrawer::placeComponents(void) {
  pimpl->placeComponents();
}


void ResultDrawer::removeComponents(void) {
  pimpl->removeComponents();

  // 音楽の停止
  pimpl->common_->haltMusic();
}


bool ResultDrawer::keyPressed(void) {

  pimpl->common_->input_receiver_->updateInputEvent(pimpl->input_event_);
  pimpl->is_decided_ |=
    InputEvent::isReleased(pimpl->input_event_, SDLK_RETURN);

  return pimpl->is_decided_;
}
