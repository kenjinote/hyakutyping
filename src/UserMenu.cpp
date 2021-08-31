/*!
  \file
  \brief ユーザメニュー

  \author Satofumi KAMIMURA

  $Id$
*/

#include "UserMenu.h"
#include "UserMenu_uni.h"
#include "ResourceDefinition.h"
#include "DrawsDefinition.h"
#include "CommonResources.h"
#include "CreateUser.h"
#include "RemoveUser.h"
#include "UpdateUtils.h"
#include "Layer.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "LabelComponent.h"
#include "ButtonComponent.h"
#include "MenuComponent.h"
#include "SdlUtils.h"
#include "Delay.h"

using namespace beego;


struct UserMenu::pImpl {
  CommonResources* common_;
  TextProperty text_property_;
  Surface title_surface_;
  Component title_label_;
  boost::shared_ptr<MenuComponent> menu_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      text_property_(common_->font_, UserMenu_title,
                     TitleSize, Fore, Back, true),
      title_surface_(new TextSurface(text_property_)),
      title_label_(new LabelComponent(title_surface_)),
      menu_(new MenuComponent) {

    // タイトルの配置
    SDL_Rect position;
    set_SdlRect(&position, centerPosition(title_label_, 640/2),
                middlePosition(title_label_, 480/4));
    title_label_->setPosition(&position);

    // メニュー項目の作成
    const Uint16 *titles[] = {
      UserMenu_add, UserMenu_delete, UserMenu_back,
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

    set_SdlRect(&position, centerPosition(menu_, 640/2), 480/4 + MenuOffset);
    menu_->setPosition(&position);
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(menu_);
    common_->front_layer_->push_front(common_->user_label_);
  }

  void removeComponents(void) {
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(menu_);
    common_->front_layer_->remove(common_->user_label_);
  }
};


UserMenu::UserMenu(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


UserMenu::~UserMenu(void) {
  pimpl->removeComponents();
}


void UserMenu::run(void) {

  // ユーザ名の表示
  //pimpl->common_->draw

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    // !!! 強制的に、ユーザ削除に移動するようにする
    int selected = pimpl->menu_->getDecided();
    if (0 || selected >= 0) {
      //selected = 1;

      // 決定音を鳴らす
      pimpl->common_->playEffect(Decide);

      pimpl->removeComponents();
      if (selected == 0) {
        // 新規ユーザの作成
        CreateUser create_user;
        create_user.run();

      } else if (selected == 1) {
        // ユーザの削除
        RemoveUser remove_user;
        remove_user.run();

      } else if (selected == 2) {
        // 戻る
        return;
      }
      pimpl->common_->updateUserLable();

      // メニューを再選択可能にする
      pimpl->placeComponents();
      pimpl->menu_->releaseDecided();
    }

    quit |= updater.update();
    delay(1);
  }
}
