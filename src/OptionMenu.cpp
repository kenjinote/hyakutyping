/*!
  \file
  \brief �I�v�V�������j���[

  \author Satofumi KAMIMURA

  $Id$

  \todo ��ԉ��̍��ڂ��A�u�߂�v�ɂ���
  \todo �߂�A�ɓ��ꂷ��
  \todo �^�C�s���O�̕\���ŁA���܂莚�������\�����邩�A�̃I�v�V�������ڂ�ǉ�
  \todo �h���b�O�𗣂�����A������悤�ɂ���
  \todo ���F�̍��ڂ͖ڗ����Ȃ��̂��A�Ȃ�Ƃ�����
  \todo VolumeSliderComponent ���{�����[���ύX�p���ۂ�����
  \todo Pink �F���킩��ɂ����̂��A�Ȃ�Ƃ�����
  \todo �R���{�{�b�N�X���A�R�s�ȏ゠��Ƃ��ɁA�����Ƃ��ɕ`�悪�c��̂��Ȃ�Ƃ�����
  \todo ���ڂ̐����̓|�b�v�A�b�v�ōs��
  \todo "guest" ���[�U�̂Ƃ��́A�T�[�o�ւ̃f�[�^���M���ł��Ȃ����Ƃ�m�点��
  \todo �`�F�b�N�{�b�N�X�̐F�ݒ�𒲐�����
  \todo VolumeSliderComponent �����������i�D�悭����
  \todo VolumeSliderComponent �ŁA�h���b�O���͓��͂𗣂��Ȃ��悤�ɂ�����
  \todo ���ʂ�ύX�����Ƃ��ɁA���ʉ�����悤�ɂ���
*/

#include "OptionMenu.h"
#include "OptionMenu_uni.h"
#include "PracticeNum_uni.h"
#include "speaker_02_bmp.h"
#include "white_bmp.h"
#include "arrow_bmp.h"
#include "CreateSurfaceFromArray.h"
#include "DrawsDefinition.h"
#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "UpdateUtils.h"
#include "AccessSettings.h"
#include "Layer.h"
#include "OptionMenuComponent.h"
#include "SdlSurface.h"
#include "MultiSurface.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "LabelComponent.h"
#include "SelectComponent.h"
#include "CheckBoxComponent.h"
#include "VolumeSliderComponent.h"
#include "ComboBoxComponent.h"
#include "ButtonComponent.h"
#include "SdlUtils.h"
#include "Delay.h"

using namespace beego;


struct OptionMenu::pImpl {

  enum {
    OptionSize = NormalSize - 1, //!< �I�v�V�����̃T�C�Y
    PlaceOffset = 170,           //!< �R���|�[�l���g�̔z�u�I�t�Z�b�g
    ItemsOffset = 25,            //!< �I�����ڂ̔z�u�Ԋu
    TitleX = 50,                 //!< ���ڃ^�C�g���̈ʒu
    PlaceY = 95,                 //!< �I�v�V�����̔z�u�J�n�ʒu
    VLength = 260,               //!< �{�����[���X���C�_�[��
  };

  CommonResources* common_;
  size_t user_index_;
  std::vector<std::string> voice_resources_;
  boost::shared_ptr<OptionMenuComponent> option_;
  Surface arrow_surface_;
  Component arrow_label_;
  TextProperty base_property_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  TextProperty escape_property_;
  TextProperty escape_focus_property_;
  boost::shared_ptr<ButtonComponent> escape_button_;
  TextProperty typing_property_;
  Surface typing_surface_;
  Component typing_label_;
  TextProperty sound_property_;
  Surface sound_surface_;
  Component sound_label_;
  TextProperty system_property_;
  Surface system_surface_;
  Component system_label_;
  boost::shared_ptr<SelectComponent> practice_num_selector_;
  boost::shared_ptr<SelectComponent> practice_input_selector_;
  boost::shared_ptr<CheckBoxComponent> practice_eachtime_cbox_;
  boost::shared_ptr<VolumeSliderComponent> sound_music_vslider_;
  boost::shared_ptr<VolumeSliderComponent> sound_effect_vslider_;
  boost::shared_ptr<ComboBoxComponent> sound_voice_combobox_;
  boost::shared_ptr<CheckBoxComponent> system_full_cbox_;
  boost::shared_ptr<CheckBoxComponent> system_send_cbox_;
  Surface speaker_01_surface_;
  Component speaker_01_label_;
  Surface speaker_02_surface_;
  Surface white_bmp_surface_;
  boost::shared_ptr<MultiSurface> normal_surface_;
  boost::shared_ptr<ButtonComponent> speaker_button_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      user_index_(common_->settings_->user_index_),
      voice_resources_(common_->getAllVoiceResources()),
      option_(new OptionMenuComponent),
      arrow_surface_(new SdlSurface(createSurface(arrow_bmp,
                                                  arrow_bmp_width,
                                                  arrow_bmp_height), true)),
      arrow_label_(new LabelComponent(arrow_surface_)),
      base_property_(common_->font_, "", OptionSize, Fore, Back, true),
      title_property_(common_->font_, OptionMenu_title,
                      MenuSize, Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      escape_property_(common_->font_, OptionMenu_escape,
                       OptionSize, Fore, Back, true),
      escape_focus_property_(common_->font_, OptionMenu_escape,
                             OptionSize, Blue, Back, true),
      escape_button_(new ButtonComponent),
      typing_property_(common_->font_, OptionMenu_typing,
                       NormalSize, Fore, Back, true),
      typing_surface_(new TextSurface(typing_property_)),
      typing_label_(new LabelComponent(typing_surface_)),
      sound_property_(common_->font_, OptionMenu_sound,
                      NormalSize, Fore, Back, true),
      sound_surface_(new TextSurface(sound_property_)),
      sound_label_(new LabelComponent(sound_surface_)),
      system_property_(common_->font_, OptionMenu_system,
                      NormalSize, Fore, Back, true),
      system_surface_(new TextSurface(system_property_)),
      system_label_(new LabelComponent(system_surface_)),
      practice_num_selector_(new SelectComponent(base_property_, White)),
      practice_input_selector_(new SelectComponent(base_property_, White)),
      practice_eachtime_cbox_(new CheckBoxComponent),
      sound_music_vslider_(new VolumeSliderComponent(VLength, OptionSize)),
      sound_effect_vslider_(new VolumeSliderComponent(VLength, OptionSize)),
      sound_voice_combobox_(new ComboBoxComponent),
      system_full_cbox_(new CheckBoxComponent),
      system_send_cbox_(new CheckBoxComponent),
      speaker_01_surface_(new SdlSurface(createSurface(speaker_02_bmp,
                                                       speaker_02_bmp_width,
                                                       speaker_02_bmp_height),
                                         true)),
      speaker_01_label_(new LabelComponent(speaker_01_surface_)),
      speaker_02_surface_(new SdlSurface(createSurface(speaker_02_bmp,
                                                       speaker_02_bmp_width,
                                                       speaker_02_bmp_height),
                                         false)),
      white_bmp_surface_(new SdlSurface(createSurface(white_bmp,
                                                      white_bmp_width,
                                                      white_bmp_height), true)),
      normal_surface_(new MultiSurface(white_bmp_width, white_bmp_height)),
      speaker_button_(new ButtonComponent)
  {

    SDL_Rect position;

    // �u�I�v�V�����v
    set_SdlRect(&position, centerPosition(title_label_, 640/2), TextOffset);
    title_label_->setPosition(&position);

    // �^�C�s���O�ݒ�
    set_SdlRect(&position, TitleX, PlaceY - (NormalSize + TextOffset/2));
    typing_label_->setPosition(&position);

    // - ���K��
    TextProperty practice_num(common_->font_, OptionMenu_typing_num,
                              OptionSize, Fore, Back, true);
    practice_num_selector_->addItem(PracticeNum_5);
    practice_num_selector_->addItem(PracticeNum_10);
    practice_num_selector_->addItem(PracticeNum_50);
    practice_num_selector_->addItem(PracticeNum_100);
    practice_num_selector_->setItemsOffset(ItemsOffset);

    int index = common_->getPracticeNumIndex();
    practice_num_selector_->setSelected(index);
    option_->addItem(practice_num, practice_num_selector_, PlaceOffset, Middle);

    // - ���̓��[�h
    TextProperty practice_input(common_->font_, OptionMenu_input,
                                OptionSize, Fore, Back, true);
    practice_input_selector_->addItem(OptionMenu_Roman);
    practice_input_selector_->addItem(OptionMenu_Kana);
    practice_input_selector_->setItemsOffset(ItemsOffset);
    index = (common_->getConvertType() == TypingTextConverter::Roman) ? 0 : 1;
    practice_input_selector_->setSelected(index);
    option_->addItem(practice_input,
                     practice_input_selector_, PlaceOffset, Middle);

    // - ���񓯂�����o�肷��
    TextProperty practice_mode(common_->font_, OptionMenu_eachtime_title,
                               OptionSize, Fore, Back, true);
    TextProperty practice_eachtime(common_->font_, OptionMenu_eachtime,
                                   OptionSize, Fore, Back, true);
    Surface practice_eachtime_surface(new TextSurface(practice_eachtime));
    practice_eachtime_cbox_->setSurface(practice_eachtime_surface, 24, 0);
    practice_eachtime_cbox_->
      setCheck((common_->getPracticeWakaNo() >= 0) ? true : false);
    practice_eachtime_cbox_->isCheckChanged();
    option_->addItem(practice_mode,
                     practice_eachtime_cbox_, PlaceOffset, Middle);

    // ���y�A���ʉ��A�r�ݏグ
    position.y += (OptionSize + TextOffset) * 4 + (TextOffset * 2);
    sound_label_->setPosition(&position);

    option_->addItemOffset(0, OptionSize + (TextOffset * 3));

    // - �w�i��
    TextProperty sound_music(common_->font_, OptionMenu_music,
                             OptionSize, Fore, Back, true);
    int volume = common_->settings_->music_volume_[user_index_];
    sound_music_vslider_->setValue(volume);
    option_->addItem(sound_music, sound_music_vslider_, PlaceOffset, Middle);

    // - ���ʉ�
    TextProperty sound_effect(common_->font_, OptionMenu_effect,
                             OptionSize, Fore, Back, true);
    volume = common_->settings_->effect_volume_[user_index_];
    sound_effect_vslider_->setValue(volume);
    option_->addItem(sound_effect, sound_effect_vslider_, PlaceOffset, Middle);

    // - �r�ݏグ
    TextProperty sound_voice(common_->font_, OptionMenu_voice,
                             OptionSize, Fore, Back, true);
    sound_voice_combobox_->setFrameColor(White);
    sound_voice_combobox_->setDecidedBackgroundColor(White);
    sound_voice_combobox_->setSelectedBackgroundColor(Blue);
    sound_voice_combobox_->setRightOffset(TextOffset);

    std::vector<std::string> texts = voice_resources_;
    TextProperty normal(common_->font_, "", OptionSize - 4,
                        Fore, White, true);
    TextProperty selected(common_->font_, "", OptionSize - 4,
                          White, Blue, true);
    ComboBoxComponent::createComboBox(sound_voice_combobox_,
                                      texts, normal, selected, White);

    // �����I���C���f�b�N�X�̌���
    std::string voice_resource_name = common_->getVoiceResource();
    // !!! data_CD-ROM.txt ���w�肷��ƁA��܂��������ɂȂ邪�A�ۗ�
    index = 0;
    for (std::vector<std::string>::iterator it = texts.begin();
         it != texts.end(); ++it, ++index) {
      if (! it->compare(voice_resource_name)) {
        sound_voice_combobox_->setSelected(index);
        break;
      }
    }
    option_->addItem(sound_voice, sound_voice_combobox_, PlaceOffset, Middle);

    // �V�X�e��
    position.y += (OptionSize + TextOffset) * 4 + (TextOffset * 2);
    system_label_->setPosition(&position);

    option_->addItemOffset(0, OptionSize + (TextOffset * 3));

#if 0
    // !!! �Ƃ肠�����A�����
    // - ��ʃ��[�h
    TextProperty system_full_title(common_->font_, OptionMenu_full_title,
                               OptionSize, Fore, Back, true);
    TextProperty system_full(common_->font_, OptionMenu_full,
                                     OptionSize, Fore, Back, true);
    Surface system_full_surface(new TextSurface(system_full));
    system_full_cbox_->setSurface(system_full_surface, 24, 0);
    system_full_cbox_->setCheck(false);
    option_->addItem(system_full_title, system_full_cbox_, PlaceOffset, Middle);
#endif

    // - �^�C�s���O�f�[�^���T�[�o�ɑ��M
    TextProperty system_send_title(common_->font_, OptionMenu_send_title,
                               OptionSize, Fore, Back, true);
    TextProperty system_send(common_->font_, OptionMenu_send,
                                     OptionSize, Fore, Back, true);
    Surface system_send_surface(new TextSurface(system_send));
    system_send_cbox_->setSurface(system_send_surface, 24, 0);
    system_send_cbox_->setCheck(common_->isSendData());
    system_send_cbox_->isCheckChanged();
    option_->addItem(system_send_title, system_send_cbox_, PlaceOffset, Middle);

    // �Đ��{�^���̍쐬
    normal_surface_->registerSurface(speaker_01_surface_);
    normal_surface_->registerSurface(white_bmp_surface_);
    speaker_button_->registerNormalSurface(normal_surface_);
    speaker_button_->registerPressedSurface(speaker_02_surface_);
    speaker_button_->registerFocusedSurface(speaker_02_surface_);

    // �߂�{�^���̍쐬
    ButtonComponent::createButton(escape_button_, escape_property_,
                                  escape_focus_property_,
                                  escape_focus_property_);

    // �I�v�V�����̔z�u
    option_->setIconComponent(arrow_label_, -24, 0, Middle);
    option_->setItemsOffset(0, OptionSize + TextOffset);
    set_SdlRect(&position, centerPosition(option_, 640/2), PlaceY);
    option_->setPosition(&position);

    // ���ʉ��̍Đ��{�^���̔z�u
    set_SdlRect(&position, position.x + option_->getWidth(),
                PlaceY + (OptionSize + TextOffset) * 4
                + (OptionSize + (TextOffset * 3)) - 3);
    speaker_button_->setPosition(&position);

    // �uESC �Ŗ߂�܂��v
    set_SdlRect(&position, rightPosition(escape_button_, 640) - TextOffset,
                bottomPosition(escape_button_, 480) - TextOffset);
    escape_button_->setPosition(&position);

    // �x���ɑΏ�
    //if (OptionMenu_full_title || OptionMenu_full);
    //if (PracticeNum_left || PracticeNum_syu || PracticeNum_complete);
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(sound_label_);
    common_->front_layer_->push_front(system_label_);
    common_->front_layer_->push_front(escape_button_);
    common_->front_layer_->push_front(typing_label_);
    common_->front_layer_->push_front(option_);
    common_->front_layer_->push_front(speaker_button_);
    common_->front_layer_->push_front(common_->user_label_);
  }

  void removeComponents(void) {
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(sound_label_);
    common_->front_layer_->remove(system_label_);
    common_->front_layer_->remove(escape_button_);
    common_->front_layer_->remove(typing_label_);
    common_->front_layer_->remove(option_);
    common_->front_layer_->remove(speaker_button_);
    common_->front_layer_->remove(common_->user_label_);
  }

  // �ύX�̔��f
  void updateInput(void) {

    // ���͐��̔��f
    int decided = practice_num_selector_->getDecided();
    if (decided >= 0) {
      int num_array[] = { 5, 10, 50, 100 };
      common_->settings_->practice_num_[user_index_] = num_array[decided];
      practice_num_selector_->releaseDecided();
    }

    // ���̓��[�h�̕ύX
    decided = practice_input_selector_->getDecided();
    if (decided >= 0) {
      common_->settings_->convert_mode_[user_index_] = decided;
      practice_input_selector_->releaseDecided();
    }

    // �{�����[���̕ύX
    if (sound_music_vslider_->isChanged()) {
      int volume = sound_music_vslider_->getValue();
      common_->settings_->music_volume_[user_index_] = volume;
      common_->updateSoundVolume(volume);
    }
    if (sound_effect_vslider_->isChanged()) {
      int volume = sound_effect_vslider_->getValue();
      common_->settings_->effect_volume_[user_index_] = volume;
      common_->updateEffectVolume(volume);
    }

    // ����A��������o�肷��
    if (practice_eachtime_cbox_->isCheckChanged()) {

      // �O��܂ł̘a�̔ԍ���ۑ�����
      int current = common_->settings_->practice_mode_[user_index_];
      int next = -current - 1;
      common_->settings_->practice_mode_[user_index_] = next;
    }

    // �f�[�^�𑗐M����
    if (system_send_cbox_->isCheckChanged()) {
      common_->settings_->data_send_[user_index_] =
        (system_send_cbox_->isChecked()) ? 0 : 1;
    }

    // ���ʉ���炷
    if (speaker_button_->isDecided()) {
      common_->playEffect(Decide);
      speaker_button_->releaseDecided();
    }

    // �r�ݏグ���\�[�X
    if (sound_voice_combobox_->isSelectChanged()) {
      int selected = sound_voice_combobox_->getSelected();
      // CD-ROM �w��� "" �Ƃ���
      common_->settings_->voice_resource_[user_index_] =
        (selected == 0) ? "" : voice_resources_[selected];
    }
  }
};


OptionMenu::OptionMenu(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


OptionMenu::~OptionMenu(void) {
  pimpl->removeComponents();
  pimpl->common_->settings_->save();
}


void OptionMenu::run(void) {

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    // �ύX�̊Ď��A���f
    pimpl->updateInput();
    quit |= updater.update() | pimpl->escape_button_->isDecided();
    delay(1);
  }
}
