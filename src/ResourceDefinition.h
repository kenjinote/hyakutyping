#ifndef RESOURCE_DEFINITION_H
#define RESOURCE_DEFINITION_H

/*!
  \file
  \brief ���\�[�X��`

  \author Satofumi KAMIMURA

  $Id$
*/

#include "DetectOS.h"


// �ݒ�t�@�C��
const char*const ConfigFile = "hyakutypingrc.txt"; //!< �ݒ�t�@�C��


/*!
  \brief �f�t�H���g�̃T�[�o�A�h���X
*/
#ifndef DEFAULT_SERVER_ADDRESS
const char*const DefaultServerAddress = "hyakutyping.sourceforge.jp/";
#else
const char*const DefaultServerAddress = DEFAULT_SERVER_ADDRESS;
#endif


// ���y�܂��
enum {
  BackMusic_1 = 1,              //!< �w�i�ȂP
  BackMusic_2,                  //!< �w�i�ȂQ
  SoundEffect_1,                //!< ���ʉ��P
  SoundEffect_2,                //!< ���ʉ��Q
  Decide,                       //!< ���艹
  Type,                         //!< �^�C�v��
  Miss,                         //!< �~�X��

  FloatingEffectId,             //!< �C�ӂ̌��ʉ�
};


// �ۑ��f�B���N�g��
#if !defined(PACKAGE_DATA_DIR) || defined(WINDOWS_OS)
const char*const FontPath = "resource/font.ttf"; //!< �t�H���g�p�X
const char*const SaveDir = "savedata/"; //!< �ۑ��f�B���N�g��
const char*const VoiceDir = "voicedata/"; //!< �r�ݏグ�p
const char*const MusicDirectory = "resource/"; //!< ���y�����̏ꏊ
const char*const Music1_FilePath = "resource/music1.mp3"; //!< �w�i�ȂP
const char*const Music2_FilePath = "resource/music2.mp3"; //!< �w�i�ȂQ
const char*const Effect1_FilePath = "resource/effect1.wav"; //!< �J�n��
const char*const Effect2_FilePath = "resource/effect2.wav"; //!< �I����
const char*const Decide_FilePath = "resource/decide.wav";   //!< ���艹
const char*const Miss_FilePath = "resource/miss.wav";       //!< �~�X��

#else
const char*const FontPath = PACKAGE_DATA_DIR "/font.ttf"; //!< �t�H���g�p�X
const char*const SaveDir = "~/.hyakutyping/"; //!< �ۑ��f�B���N�g��
const char*const VoiceDir = PACKAGE_DATA_DIR "/";      //!< �r�ݏグ�p
const char*const MusicDirectory = PACKAGE_DATA_DIR "/";//!< ���y�����̏ꏊ
const char*const Music1_FilePath = PACKAGE_DATA_DIR "/music1.mp3"; //!< �w�i�ȂP
const char*const Music2_FilePath = PACKAGE_DATA_DIR "/music2.mp3";//!< �w�i�ȂQ
const char*const Effect1_FilePath = PACKAGE_DATA_DIR "/effect1.wav"; //!< �J�n��
const char*const Effect2_FilePath = PACKAGE_DATA_DIR "/effect2.wav"; //!< �I����
const char*const Decide_FilePath = PACKAGE_DATA_DIR "/decide.wav"; //!< ���艹
const char*const Miss_FilePath = PACKAGE_DATA_DIR "/miss.wav";     //!< �~�X��
#endif

#endif /* !RESOURCE_DEFINITION_H */
