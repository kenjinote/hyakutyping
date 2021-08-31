/*!
  \file
  \brief �N����̃��j���[���

  \author Satofumi KAMIMURA

  $Id$

  \todo �^�u���͂ŁA�R���{�{�b�N�X���J��
  \todo �|�b�v�A�b�v�Ń��j���[���ڂ̐������s��
  \todo ���j���[�̐���������������
*/

#include "FirstMenu.h"
#include "FirstMenu_uni.h"
#include "hyakutyping.h"
#include "DrawsDefinition.h"
#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "UpdateUtils.h"
#include "PracticeMenu.h"
#include "NetworkMenu.h"
#include "ViewHistory.h"
#include "OptionMenu.h"
#include "UserMenu.h"
#include "user_bmp.h"
#include "white_bmp.h"
#include "AccessSettings.h"
#include "Layer.h"
#include "SdlSurface.h"
#include "MultiSurface.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "LabelComponent.h"
#include "ButtonComponent.h"
#include "MenuComponent.h"
#include "ComboBoxComponent.h"
#include "CreateSurfaceFromArray.h"
#include "SdlUtils.h"
#include "Delay.h"

using namespace beego;


struct FirstMenu::pImpl {
  CommonResources* common_;
  TtfResource* font_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  boost::shared_ptr<MenuComponent> menu_;
  TextProperty version_property_;
  Surface version_surface_;
  Component version_label_;

  Surface user_bmp_surface_;
  Surface white_bmp_surface_;
  boost::shared_ptr<MultiSurface> focus_surface_;
  boost::shared_ptr<ButtonComponent> user_button_;

  boost::shared_ptr<ComboBoxComponent> user_combobox_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      title_property_(common_->font_, FirstMenu_title, TitleSize,
                      Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      menu_(new MenuComponent),
      version_property_(common_->font_, VersionText, NormalSize,
                        Fore, Back, true),
      version_surface_(new TextSurface(version_property_)),
      version_label_(new LabelComponent(version_surface_)),
      user_bmp_surface_(new SdlSurface(createSurface(user_bmp,
                                                     user_bmp_width,
                                                     user_bmp_height), true)),
      white_bmp_surface_(new SdlSurface(createSurface(white_bmp,
                                                      white_bmp_width,
                                                      white_bmp_height), true)),
      focus_surface_(new MultiSurface(white_bmp_width, white_bmp_height)),
      user_button_(new ButtonComponent),
      user_combobox_(new ComboBoxComponent)
  {

    // �o�[�W�����̈ʒu�ݒ�
    SDL_Rect position;
    set_SdlRect(&position, TextOffset, TextOffset);
    version_label_->setPosition(&position);

    // �^�C�g���̈ʒu�ݒ�
    set_SdlRect(&position, centerPosition(title_label_, 640/2),
                middlePosition(title_label_, 480/4));
    title_label_->setPosition(&position);

    // ���j���[���ڂ̍쐬
    const Uint16 *titles[] = {
      FirstMenu_practice, FirstMenu_network, FirstMenu_history,
      FirstMenu_option, FirstMenu_quit,
    };

    // !!! ���̃��j���[����鏈���́ACommonResource ���̊֐��ŏ��������悤
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

    // ���[�U�{�^���̍쐬
    user_button_->registerNormalSurface(user_bmp_surface_);
    focus_surface_->registerSurface(user_bmp_surface_);
    focus_surface_->registerSurface(white_bmp_surface_);
    user_button_->registerFocusedSurface(focus_surface_);
    user_button_->registerPressedSurface(focus_surface_);

    // �R���{�{�b�N�X�̐ݒ�
    user_combobox_->setFrameColor(Pink);
    user_combobox_->setDecidedBackgroundColor(Pink);
    user_combobox_->setSelectedBackgroundColor(Pink);
    user_combobox_->setRightOffset(TextOffset);

    set_SdlRect(&position, centerPosition(menu_, 640/2), 480/4 + MenuOffset);
    menu_->setPosition(&position);

    common_->back_layer_->push_front(common_->back_label_);

    set_SdlRect(&position,
                rightPosition(user_button_, 640 - TextOffset), TextOffset / 2);
    user_button_->setPosition(&position);
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(version_label_);
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(menu_);
    common_->front_layer_->push_front(user_button_);
    if (! common_->settings_->user_names_.empty()) {
      common_->front_layer_->push_front(user_combobox_);
    }
  }

  void removeComponents(void) {
    common_->front_layer_->remove(version_label_);
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(menu_);
    common_->front_layer_->remove(user_button_);
    common_->front_layer_->remove(user_combobox_);
  }

  void updateUserComboBox(void) {

    // �R���{�{�b�N�X�̍č\�z
    user_combobox_->clearItems();

    TextProperty normal(common_->font_, "", NormalSize, Fore, Pink, false);
    TextProperty selected(common_->font_, "", NormalSize, Blue, Pink, false);

    for (std::vector<std::string>::iterator it
           = common_->settings_->user_names_.begin();
         it != common_->settings_->user_names_.end(); ++it) {
      normal.text = it->c_str();
      selected.text = it->c_str();

      // ���ڂ̓o�^
      boost::shared_ptr<ButtonComponent> new_button(new ButtonComponent);
      ButtonComponent::createButton(new_button, normal, selected, selected);
      user_combobox_->addItem(new_button);
    }

    // �����C���f�b�N�X���w��
    user_combobox_->setSelected
      (static_cast<int>(common_->settings_->user_index_));

    // �ʒu�ݒ�
    SDL_Rect position;
    set_SdlRect(&position,
                rightPosition(user_combobox_, 640) - TextOffset, TextOffset);
    user_combobox_->setPosition(&position);

    position.x -= TextOffset + static_cast<int>(user_button_->getWidth());
    position.y = TextOffset / 2;
    user_button_->setPosition(&position);
  }

  void playDecideEffect(int selected) {

    // !!! �I��s�\�ȍ��ڂ����肵���Ƃ��ɉ���ς������Ȃ�����A�ύX����
    if (selected != 4) {
      // �u�I���v�̂Ƃ��͌��ʉ���炳�Ȃ�
      common_->playEffect(Decide);
    }
  }
};


FirstMenu::FirstMenu(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


FirstMenu::~FirstMenu(void) {
  pimpl->removeComponents();
}


void FirstMenu::run(void) {

  // ���[�U���\���̃R���{�{�b�N�X���Đ���
  pimpl->common_->updateUserLable();
  pimpl->updateUserComboBox();

  // ���y�̍Đ�
  pimpl->common_->setNextMusic(BackMusic_1);

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    // �R���{�{�b�N�X����ɂ�郆�[�U���̕ύX
    if (pimpl->user_combobox_->isSelectChanged()) {
      pimpl->common_->changeUserIndex(pimpl->user_combobox_->getSelected());
      pimpl->common_->settings_->save();
    }

    int selected = pimpl->menu_->getDecided();
    bool user_config = pimpl->user_button_->isDecided();
    if (0 || (selected >= 0) || user_config) {
      //selected = 3;

      // ���艹��炷
      pimpl->playDecideEffect(selected);
      pimpl->removeComponents();

      if (user_config) {
        // ���[�U�Ǘ��{�^���������ꂽ�Ƃ��̏���
        UserMenu user;
        user.run();

        // ���[�U���R���{�{�b�N�X�̍X�V
        pimpl->updateUserComboBox();

        // ���j���[�̑I�����ڂ�����������
        selected = 0;

      } else if (selected == 0) {
        // �^�C�s���O���K
        PracticeMenu practice;
        practice.run();

      } else if (selected == 1) {
        // �l�b�g���[�N�ΐ�
        NetworkMenu network;
        network.run();

      } else if (selected == 2) {
        // �^�C�s���O�����̉{��
        ViewHistory history;
        history.run();

      } else if (selected == 3) {
        // �I�v�V����
        OptionMenu option;
        option.run();

      } else if (selected == 4) {
        // �Q�[���̏I��
        return;
      }

      // ���j���[���đI���\�ɂ���
      pimpl->user_button_->releaseDecided();
      pimpl->menu_->releaseDecided();
      pimpl->menu_->setSelected(selected);
      pimpl->placeComponents();
    }

    quit |= updater.update();
    delay(1);
  }
}
