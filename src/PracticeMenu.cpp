/*!
  \file
  \brief タイピング練習のメニュー

  \author Satofumi KAMIMURA

  $Id$
*/

#include "PracticeMenu.h"
#include "PracticeMenu_uni.h"
#include "PracticeNum_uni.h"
#include "DrawsDefinition.h"
#include "SystemDefinition.h"
#include "ShimonokuPractice.h"
#include "KimarijiPractice.h"
#include "CommonResources.h"
#include "UpdateUtils.h"
#include "AccessSettings.h"
#include "ResourceDefinition.h"
#include "Layer.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "LabelComponent.h"
#include "MenuComponent.h"
#include "ButtonComponent.h"
#include "ComboBoxComponent.h"
#include "SdlUtils.h"
#include "Delay.h"

using namespace beego;


struct PracticeMenu::pImpl {
  CommonResources* common_;
  TtfResource* font_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  boost::shared_ptr<MenuComponent> menu_;
  boost::shared_ptr<ComboBoxComponent> num_combobox_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      font_(common_->font_),
      title_property_(font_, PracticeMenu_title, TitleSize,
                      Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      menu_(new MenuComponent),
      num_combobox_(new ComboBoxComponent) {

    SDL_Rect position;
    // !!! 640, 480 まわりの定数は置き換える
    set_SdlRect(&position, centerPosition(title_label_, 640/2),
                middlePosition(title_label_, 480/4));
    title_label_->setPosition(&position);

    // メニュー内の項目を作成
    const Uint16 *titles[] = {
      PracticeMenu_shimonoku, PracticeMenu_kimariji, PracticeMenu_back,
    };

    size_t max_width = 0;
    for (size_t i = 0; i < sizeof(titles)/sizeof(titles[0]); ++i) {
      boost::shared_ptr<ButtonComponent> new_button(new ButtonComponent);
      TextProperty normal(common_->font_, titles[i], MenuSize,
                          Fore, Back, true);
      TextProperty selected(common_->font_, titles[i], MenuSize,
                            Selected, Back, true);
      ButtonComponent::createButton(new_button, normal, selected, selected);
      if (new_button->getWidth() > max_width) {
        max_width = new_button->getWidth();
      }
      menu_->addItem(new_button);
    }
    menu_->setItemsOffset(0, MenuSize + 16);
    menu_->setItemSelectWidth(max_width);
    menu_->setSelected(0);
    menu_->setIconComponent(common_->menu_icon_, IconOffset, 0, Middle | Left);

    // コンボボックスの作成
    num_combobox_->setFrameColor(Pink);
    num_combobox_->setDecidedBackgroundColor(Pink);
    num_combobox_->setSelectedBackgroundColor(Pink);
    num_combobox_->setRightOffset(TextOffset);

    Uint16 dummy[] = { 0x0 };
    TextProperty normal(common_->font_, dummy, NormalSize, Fore, Pink, false);
    TextProperty selected(common_->font_, dummy, NormalSize, Blue, Pink, false);

    const Uint16* num_text[] = {
      PracticeNum_5, PracticeNum_10, PracticeNum_50, PracticeNum_100,
    };
    for (size_t i = 0; i < sizeof(num_text)/sizeof(num_text[0]); ++i) {
      normal.utext = num_text[i];
      selected.utext = num_text[i];
      boost::shared_ptr<ButtonComponent> new_button(new ButtonComponent);
      ButtonComponent::createButton(new_button, normal, selected, selected);
      num_combobox_->addItem(new_button);
    }
    num_combobox_->setSelected(common_->getPracticeNumIndex());

    // コンポーネントの配置
    set_SdlRect(&position, centerPosition(menu_, 640/2), 480/4 + MenuOffset);
    menu_->setPosition(&position);

    // 位置設定
    set_SdlRect(&position,
                rightPosition(num_combobox_, 640) - TextOffset,
                TextOffset + NormalSize + TextOffset);
    num_combobox_->setPosition(&position);

    // 警告に対処
    //if (PracticeNum_left && PracticeNum_syu && PracticeNum_complete);
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(menu_);
    common_->front_layer_->push_front(common_->user_label_);
    common_->front_layer_->push_front(num_combobox_);
  }

  void removeComponents(void) {
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(menu_);
    common_->front_layer_->remove(common_->user_label_);
    common_->front_layer_->remove(num_combobox_);
  }

  // 音楽の停止
  void stopMusic(void) {
    common_->stopMusic(StopFadeOut);
  }

  void playMusic(void) {
    common_->setNextMusic(BackMusic_1);
  }
};


PracticeMenu::PracticeMenu(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


PracticeMenu::~PracticeMenu(void) {
  pimpl->removeComponents();
}


void PracticeMenu::run(void) {

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    // コンボボックス操作による練習数の変更
    if (pimpl->num_combobox_->isSelectChanged()) {
      pimpl->common_->setPracticeNumIndex(pimpl->num_combobox_->getSelected());
      pimpl->common_->settings_->save();
    }

    int selected = pimpl->menu_->getDecided();
    if (selected >= 0) {

      // 決定音を鳴らす
      pimpl->common_->playEffect(Decide);

      pimpl->removeComponents();
      if (selected == 0) {
        // 下の句タイピング
        pimpl->stopMusic();
        ShimonokuPractice shimonoku_practice;
        shimonoku_practice.run();
        pimpl->playMusic();

      } else if (selected == 1) {
        // 決まり字タイピング
        pimpl->stopMusic();

        KimarijiPractice kimariji_practice;
        kimariji_practice.run();
        pimpl->playMusic();

      } else if (selected == 2) {
        // 戻る
        quit |= true;
      }

      // メニューを再選択可能にする
      pimpl->placeComponents();
      pimpl->menu_->releaseDecided();
    }

    quit |= updater.update();
    if (updater.haveQuitEvent()) {
      pimpl->common_->front_layer_->disable();
      quit |= true;
    }
    delay(1);
  }
}
