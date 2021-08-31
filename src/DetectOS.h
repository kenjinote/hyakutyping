#ifndef DETECT_OS_H
#define DETECT_OS_H

/*!
  \file
  \brief ����OS�̌��o

  \author Satofumi KAMIMURA

  \todo MAC OS �̌��o��ǉ�����
  \todo MinGW �̓K�؂Ȍ��o���@�𒲂ׂĔ��f����

  $Id$
*/

#if defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__
#define WINDOWS_OS

#if defined _MSC_VER
#define MSC
#elif defined __CYGWIN__
#define Cygwin
#elif defined __MINGW32__
#define MinGW
#endif

#elif defined __linux__
#define LINUX_OS

#else
#define MAC_OS
#endif

#endif /* !DETECT_OS_H */
