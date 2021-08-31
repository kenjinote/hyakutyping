/*!
  \file
  \brief ファイル有無の確認

  access() のラッパー

  \author Satofumi KAMIMURA
*/

#include "ExistFile.h"
#include "DetectOS.h"
#if defined(LINUX_OS) || defined(Cygwin) || defined(MAC_OS)
#include <unistd.h>
#else
#include <windows.h>
#include <direct.h>
#include <io.h>
#ifndef F_OK
#define F_OK 06
#endif
#ifndef W_OK
#define W_OK 06
#endif
#endif


bool beego::existFile(const char* file_path) {
  return (access(file_path, F_OK) == 0) ? true : false;
}
