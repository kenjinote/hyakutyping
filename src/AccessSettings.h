#ifndef ACCESS_SETTINGS_H
#define ACCESS_SETTINGS_H

/*!
  \file
  \brief �ݒ�̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <vector>
#include <string>


/*!
  \brief �ݒ�t�@�C���̊Ǘ��N���X
*/
class AccessSettings {
  AccessSettings(void);
  AccessSettings(const AccessSettings& rhs);
  AccessSettings& operator = (const AccessSettings& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  std::vector<std::string> user_names_; //!< ���[�U��
  std::vector<std::string> user_password_; //!< �p�X���[�h
  std::vector<std::string> server_address_; //!< �T�[�o�A�h���X

  std::vector<int> convert_mode_; //!< ���̓��[�h
  std::vector<size_t> practice_num_; //!< ���K��

  std::vector<int> practice_mode_; //!< ���K���[�h
  std::vector<int> data_send_;     //!< �f�[�^���M

  std::vector<int> music_volume_; //!< �Ȃ̃{�����[��
  std::vector<int> effect_volume_; //!< ���ʉ��̃{�����[��
  std::vector<std::string> voice_resource_; //!< �r�ݏグ���\�[�X

  size_t user_index_;           //!< �I�𒆂̃��[�U�C���f�b�N�X

  std::string effect_decide_;   //!< ���艹
  std::string effect_type_;     //!< �^�C�v��
  std::string effect_miss_;     //!< �~�X�^�C�v��

  AccessSettings(const char* savedir);
  ~AccessSettings(void);

  /*!
    \brief �ēǂݍ���
  */
  void reload(void);

  /*!
    \brief �ۑ�
  */
  void save(void);

  void deleteUser(int user_index);
};

#endif /* !ACCESS_SETTINGS_H */
