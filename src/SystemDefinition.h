#ifndef SYSTEM_DEFINITION_H
#define SYSTEM_DEFINITION_H

/*!
  \file
  \brief �i�s�ɂ��ẴV�X�e����`

  \author Satofumi KAMIMURA

  $Id$
*/


// �f�t�H���g�ݒ�
enum {
  DefaultPracticeNum = 5,
  GraphHistoryMax = 30,
};


// �V�X�e���ݒ�
enum {
  UserMax = 5,
};

// �Q�[���ݒ�
enum {
#if 1
  KaminokuPlaying = 6500,
  ShimonokuPlaying = 6000,
  YomibitoPlaying = 1000,
#else
  // �f�o�b�O���p
  KaminokuPlaying = 45,
  ShimonokuPlaying = 30,
  YomibitoPlaying = 10,
#endif
  TotalPlaying = KaminokuPlaying + ShimonokuPlaying,
};

// Web �A�N�Z�X�̃^�C���A�E�g����
enum {
  WebAccessTimeout = 3000,
};

// ���ʂ̑ҋ@����
enum {
  PrologueMsec = 5100,          //!< �v�����[�O���Ԃ̑ҋ@���� [msec]
};

// ���y�̍Đ��܂��
enum {
  PlayFadeIn = 1000,
  StopFadeOut = 200,
};

#endif /* !SYSTEM_DEFINITION_H */
