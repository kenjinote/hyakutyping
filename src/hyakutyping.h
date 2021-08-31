#ifndef HYAKUTYPING_H
#define HYAKUTYPING_H

/*!
  \file
  \brief �S�l���^�C�s���O

  \author Satofumi KAMIMURA

  $Id$

  \todo OpenGL �łŃL�[���͂���������Ȃ��̂��A�Ȃ�Ƃ�����
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif


// �A����
#ifndef PACKAGE_BUG_REPORT
#define PACKAGE_BUG_REPORT "satofumi@sourceforge.jp" //!< �o�O�A����
#endif


// �o�[�W����
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "X.X.X"         //!< �o�[�W�������
#endif
const char*const VersionText = "version " PACKAGE_VERSION; //!< �o�[�W�������

#ifndef MAJOR_VERSION
#define MAJOR_VERSION "X"
#endif
#ifndef MINOR_VERSION
#define MINOR_VERSION "X"
#endif
#ifndef MICRO_VERSION
#define MICRO_VERSION "X"
#endif


// �^�C�g��
#ifndef PACKAGE_STRING
#define PACKAGE_STRING "hyakutyping X.X.X"
#endif


// �l�b�g���[�N
enum {
  ServerPort = 57577,          //!< �l�b�g���[�N�ΐ�p�̐ڑ��|�[�g�ԍ�
};

#endif /* !HYAKUTYPING_H */
