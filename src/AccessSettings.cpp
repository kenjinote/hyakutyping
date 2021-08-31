/*!
  \file
  \brief �ݒ�̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$

  \todo �o�b�N�A�b�v�t�@�C���𐶐�����
*/

#include "AccessSettings.h"
#include "ResourceDefinition.h"
#include "AccessProperty.h"
#include "SystemDefinition.h"
#include "hyakutypingrc_orig.h"
#include "ExistFile.h"
#include "CreateDirectory.h"
#include "CreateFileFromArray.h"
#include <boost/lexical_cast.hpp>
#include <cstring>

using namespace beego;
using namespace boost;


struct AccessSettings::pImpl {
  AccessProperty* property_;

  pImpl(const char* savedir) : property_(NULL) {

    // �f�[�^�ۑ��p�f�B���N�g���𐶐�����
    if (! createDirectory(savedir)) {
      // !!! �G���[���b�Z�[�W�̏o��
      return;
    }

    // �ݒ�t�@�C�����Ȃ���΁A�R�s�[���Đ�������
    std::string config_file = std::string(savedir) + ConfigFile;
    if (! existFile(config_file.c_str())) {
      createFileFromArray(config_file.c_str(), hyakutypingrc_orig,
                          sizeof(hyakutypingrc_orig) - 1);
    }
    property_ = new AccessProperty(config_file.c_str());
  }

  ~pImpl(void) {
    delete property_;
  }

  void swapUserSettings(int user_index, int remove_index) {

    // !!! initializeUserSettings �Ɠ����ɂ��ׂ�
    const char* swap_tag[] = {
      "u_%02d", "u_%02d_http", "u_%02d_password", "u_%02d_converter",
      "u_%02d_num", "u_%02d_music", "u_%02d_effect", "u_%02d_voice",
      "u_%02d_practice", "u_%02d_send",
    };

    enum { BufferMax = 256 };
    char swap_buffer0[BufferMax];
    char swap_buffer1[BufferMax];

    AccessProperty& property = *property_;
    for (size_t i = 0; i < sizeof(swap_tag)/sizeof(swap_tag[0]); ++i) {
      sprintf(swap_buffer0, swap_tag[i], user_index);
      sprintf(swap_buffer1, swap_tag[i], remove_index);

      std::swap(property[swap_buffer0], property[swap_buffer1]);
    }
  }

  void initializeUserSettings(int remove_index) {

    const char* tag[] = {
      "u_%02d", "u_%02d_http", "u_%02d_password", "u_%02d_converter",
      "u_%02d_num", "u_%02d_music", "u_%02d_effect", "u_%02d_voice",
      "u_%02d_practice", "u_%02d_send",
    };

    enum { BufferMax = 256 };
    char buffer[BufferMax];

    AccessProperty& property = *property_;
    for (size_t i = 0; i < sizeof(tag)/sizeof(tag[0]); ++i) {
      sprintf(buffer, tag[i], remove_index);
      property[buffer] = "";
    }
  }
};


AccessSettings::AccessSettings(const char* savedir)
  : pimpl(new pImpl(savedir)), user_index_(0) {
  reload();
}


AccessSettings::~AccessSettings(void) {
}


void AccessSettings::reload(void) {

  if (! pimpl->property_) {
    return;
  }

  AccessProperty& property = *pimpl->property_;

  // �V�X�e���ݒ�̓ǂݏo��
  effect_decide_ = property["effect_decide"];
  effect_type_ = property["effect_type"];
  effect_miss_ = property["effect_miss"];

  // ���[�U���̏�����
  user_names_.resize(UserMax);
  user_password_.resize(UserMax);
  server_address_.resize(UserMax);
  practice_num_.resize(UserMax);
  convert_mode_.resize(UserMax);
  practice_mode_.resize(UserMax);
  data_send_.resize(UserMax);
  music_volume_.resize(UserMax);
  effect_volume_.resize(UserMax);
  voice_resource_.resize(UserMax);
  for (int i = 0; i < UserMax; ++i) {

    // !!! �������ɁA���̂�����̏����͊֐������܂���...

    // ���[�U��
    char buffer[] = "u_xx";
    sprintf(buffer, "u_%02d", i);
    user_names_[i] = property[buffer];

    // �p�X���[�h
    char password_tag[] = "u_xx_password";
    sprintf(password_tag, "u_%02d_password", i);
    user_password_[i] = property[password_tag];

    // �o�^�A�h���X
    char http_tag[] = "u_xx_http";
    sprintf(http_tag, "u_%02d_http", i);
    server_address_[i] = property[http_tag];

    // ���K��
    char num_tag[] = "u_xx_num";
    sprintf(num_tag, "u_%02d_num", i);
    size_t num = atoi(property[num_tag].c_str());
    if (num <= 0) {
      num = 5;
    }
    practice_num_[i] = num;

    // ���̓��[�h
    char converter_tag[] = "u_xx_converter";
    sprintf(converter_tag, "u_%02d_converter", i);
    convert_mode_[i] = atoi(property[converter_tag].c_str());

    // ���K���[�h
    char practice_tag[] = "u_xx_practice";
    sprintf(practice_tag, "u_%02d_practice", i);
    practice_mode_[i] = atoi(property[practice_tag].c_str());

    // �f�[�^���M
    char send_tag[] = "u_xx_send";
    sprintf(send_tag, "u_%02d_send", i);
    data_send_[i] = atoi(property[send_tag].c_str());

    // �Ȃ̉���
    char music_tag[] = "u_xx_music";
    sprintf(music_tag, "u_%02d_music", i);
    int volume = atoi(property[music_tag].c_str());
    music_volume_[i] = volume;

    // ���ʉ��̉���
    char effect_tag[] = "u_xx_effect";
    sprintf(effect_tag, "u_%02d_effect", i);
    volume = atoi(property[effect_tag].c_str());
    effect_volume_[i] = volume;

    // �r�ݏグ���\�[�X
    char voice_tag[] = "u_xx_voice";
    sprintf(voice_tag, "u_%02d_voice", i);
    voice_resource_[i] = property[voice_tag];
  }
  user_index_ = atoi(property["u_index"].c_str());
}


void AccessSettings::save(void) {

  // !!! isChangedByOthre() ��p���āA�㏑�����Ă��悢���A�̊m�F���b�Z�[�W���o�͂��ׂ�
  // !!! �܁A���

  if (! pimpl->property_) {
    return;
  }

  AccessProperty& property = *pimpl->property_;

  // ���[�U�C���f�b�N�X�̋L�^
  property["u_index"] = lexical_cast<std::string>(user_index_);

  // ���[�U���A�p�X���[�h�̋L�^
  for (int i = 0; i < UserMax; ++i) {

    // !!! �ȍ~���֐������܂��傤
    // !!! �]�͂���΁A�^�O�܂��� load, save �ŋ��ʂɂł���悤�ɂ���

    // ���[�U��
    char name_tag[] = "u_xx";
    sprintf(name_tag, "u_%02d", i);
    property[name_tag] = user_names_[i];

    // �p�X���[�h
    char password_tag[] = "u_xx_password";
    sprintf(password_tag, "u_%02d_password", i);
    property[password_tag] = user_password_[i];

    // �o�^�A�h���X
    char http_tag[] = "u_xx_http";
    sprintf(http_tag, "u_%02d_http", i);
    property[http_tag] = server_address_[i];

    // ���K��
    char num_tag[] = "u_xx_num";
    sprintf(num_tag, "u_%02d_num", i);
    property[num_tag] = lexical_cast<std::string>(practice_num_[i]);

    // ���̓��[�h
    char converter_tag[] = "u_xx_converter";
    sprintf(converter_tag, "u_%02d_converter", i);
    property[converter_tag] = lexical_cast<std::string>(convert_mode_[i]);

    // ���K���[�h
    char practice_tag[] = "u_xx_practice";
    sprintf(practice_tag, "u_%02d_practice", i);
    property[practice_tag] = lexical_cast<std::string>(practice_mode_[i]);

    // �f�[�^���M
    char send_tag[] = "u_xx_send";
    sprintf(send_tag, "u_%02d_send", i);
    property[send_tag] = lexical_cast<std::string>(data_send_[i]);

    // �Ȃ̉���
    char music_tag[] = "u_xx_music";
    sprintf(music_tag, "u_%02d_music", i);
    property[music_tag] = lexical_cast<std::string>(music_volume_[i]);

    // ���ʉ��̉���
    char effect_tag[] = "u_xx_effect";
    sprintf(effect_tag, "u_%02d_effect", i);
    property[effect_tag] = lexical_cast<std::string>(effect_volume_[i]);

    // �r�ݏグ���\�[�X
    char voice_tag[] = "u_xx_voice";
    sprintf(voice_tag, "u_%02d_voice", i);
    property[voice_tag] = voice_resource_[i];
  }

  property.save();
}


void AccessSettings::deleteUser(int index) {

  if (index < 0) {
    return;
  }

  // �����o�ϐ��̏�����
  // !!! �i�D������...�B������A�������������č�蒼��
  user_names_[index] = "";
  user_password_[index] = "";

  int remove_index = index;
  for (; remove_index < (UserMax - 1); ++remove_index) {
    if (strlen(user_names_[remove_index].c_str()) <= 0) {
      break;
    }
  }
  pimpl->initializeUserSettings(remove_index);
  pimpl->swapUserSettings(index, remove_index);

  if (static_cast<int>(user_index_) >= index) {
    --user_index_;
  }

  save();
}
