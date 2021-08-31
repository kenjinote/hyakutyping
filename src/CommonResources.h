#ifndef COMMON_RESOURCES_H
#define COMMON_RESOURCES_H

/*!
  \file
  \brief ���ʃ��\�[�X�̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingTextConverter.h"
#include "Component.h"
#include <string>


namespace beego {
  class GuiManager;
  class Layer;
  class TtfResource;
  class InputHandler;
  class InputReceiveComponent;
  class ConsolePanelComponent;
};
class TypingRecorder;
class AccessSettings;
class VoicePlayer;


/*!
  \brief ���ʃ��\�[�X�Ǘ�
*/
class CommonResources {
  CommonResources(void);
  CommonResources(const char* savedir);
  CommonResources(const CommonResources& rhs);
  CommonResources& operator = (const CommonResources& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  //////////////////////////////////////////////////////////////////////
  // GUI ����
  beego::GuiManager* gui_;       //!< GUI �Ǘ�
  beego::Layer* front_layer_;    //!< �O�i���C���[
  beego::Layer* back_layer_;     //!< �w�i���C���[
  boost::shared_ptr<beego::ConsolePanelComponent> debug_; //!< �f�o�b�O�\���p

  beego::Component back_label_;  //!< �w�i�摜
  beego::TtfResource* font_;     //!< �t�H���g���\�[�X
  beego::Component menu_icon_;   //!< ���j���[�A�C�R��
  beego::Component user_label_;  //!< ���[�U��

  beego::InputHandler* input_;   //!< ���̓n���h��
  boost::shared_ptr<beego::InputReceiveComponent> input_receiver_; //!< ���͎�M

  //////////////////////////////////////////////////////////////////////
  // �V�X�e��
  TypingRecorder* recorder_;     //!< �^�C�s���O���̋L�^
  std::vector<int> waka_order_;  //!< �a�̂̕���
  VoicePlayer* voice_;           //!< �r�ݏグ

  //////////////////////////////////////////////////////////////////////
  // �ݒ�
  std::string savedir_;             //!< �f�[�^�̕ۑ��f�B���N�g��
  AccessSettings* settings_;        //!< �ݒ�

  ~CommonResources(void);

  /*!
    \brief ���ʃ��\�[�X�ւ̊Ǘ��I�u�W�F�N�g��Ԃ�
  */
  static CommonResources* getObject(const char* savedir = NULL);

  /*!
    \brief GUI ���\�[�X�̏�����

    \attention �X�N���[������������Ă���Ăяo���Ȃ���΂Ȃ�Ȃ�
  */
  void initializeGui(void);

  /*!
    \brief �t�H���g�f�[�^�����邩��Ԃ�

    \retval true �t�H���g�f�[�^����
    \retval false �t�H���g�f�[�^�Ȃ�
  */
  static bool accessFontData(void);

  /*!
    \brief ���y�f�[�^�����邩��Ԃ�

    \retval true ���y�f�[�^����
    \retval false ���y�f�[�^�Ȃ�
  */
  static bool accessMusicData(void);

  /*!
    \brief ���y�����̏�����
  */
  void initializeMusic(void);

  /*!
    \brief ���݂̃��[�U���̎擾
  */
  const char* getUserName(void) const;
  int getUserIndex(void);

  const char* getPassword(void) const;

  void changeUserIndex(size_t index);

  /*!
    \brief ���[�U�̍폜

    \brief user_name [i] �ǉ����[�U��
  */
  void addUser(const char* user_name, const char* password,
               const char* address);

  /*!
    \brief �w�胆�[�U�̍폜

    \brief user_name [i] �폜���[�U�ԍ�
  */
  void deleteUser(int user_index);

  void updateUserLable(void);

  TypingTextConverter::ConvertType getConvertType(void);

  int getPracticeNum(void);

  int getPracticeNumIndex(void);
  void setPracticeNumIndex(size_t index);

  void setNextMusic(int id);
  bool nowSoundPlaying(void);
  void stopMusic(int fadeout_msec = 0);
  void haltMusic(void);
  void updateSoundVolume(int percent);

  bool registerEffect(int id, const char* file);
  int playEffect(int id);
  bool nowEffectPlaying(void);
  void stopEffect(int id = -1);
  void updateEffectVolume(int percent);

  std::string getServerName(void);
  std::string getServerAddress(void);

  int getPracticeWakaNo(void);
  bool isSendData(void);

  std::vector<std::string> getAllVoiceResources(void);
  std::string getVoiceResource(void);
};

#endif /* !COMMON_RESOURCES_H */
