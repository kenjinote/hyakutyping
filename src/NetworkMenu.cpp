/*!
  \file
  \brief �l�b�g���[�N�̃��j���[���

  \author Satofumi KAMIMURA

  $Id$
*/

#include "NetworkMenu.h"
#include "NetworkMenu_uni.h"
#include "DrawsDefinition.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "UpdateUtils.h"
#include "Layer.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "LabelComponent.h"
#include "MenuComponent.h"
#include "ButtonComponent.h"
#include "SdlUtils.h"
#include "Delay.h"

using namespace beego;


struct NetworkMenu::pImpl {
  CommonResources* common_;
  TtfResource* font_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  boost::shared_ptr<MenuComponent> menu_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      title_property_(common_->font_, NetworkMenu_title, TitleSize,
                      Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      menu_(new MenuComponent) {

    // �^�C�g���̈ʒu�ݒ�
    SDL_Rect position;
    set_SdlRect(&position, centerPosition(title_label_, 640/2),
                middlePosition(title_label_, 480/4));
    title_label_->setPosition(&position);

    // ���j���[�̐����A�ʒu�ݒ�
    const Uint16 *titles[] = {
      NetworkMenu_player, NetworkMenu_data,
      NetworkMenu_replay, NetworkMenu_back,
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


NetworkMenu::NetworkMenu(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


NetworkMenu::~NetworkMenu(void) {
  pimpl->removeComponents();
}


void NetworkMenu::run(void) {

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    int selected = pimpl->menu_->getDecided();
    if (selected >= 0) {

      // ���艹��炷
      pimpl->common_->playEffect(Decide);

      pimpl->removeComponents();
      if (selected == 0) {
        // �ΐl��
        // !!!

      } else if (selected == 1) {
        // �ߋ��f�[�^�Ƃ̑ΐ�
        // !!!

      } else if (selected == 2) {
        // �ΐ�f�[�^�̉{��
        // !!!

      } else if (selected == 3) {
        // �߂�
        quit |= true;
      }

      // ���j���[���đI���\�ɂ���
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
