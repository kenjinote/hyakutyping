#ifndef DETECT_OS_H
#define DETECT_OS_H

/*!
  \file
  \brief 動作OSの検出

  \author Satofumi KAMIMURA

  \todo MAC OS の検出を追加する
  \todo MinGW の適切な検出方法を調べて反映する

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
