/*!
  \file
  \brief ファイル削除

  \author Satofumi KAMIMURA

  $Id$

  \todo MSC 用の調整を行う
*/

#include "RemoveFile.h"
#include "DetectOS.h"
#ifndef MSC
#include <unistd.h>
#else
#include <windows.h>
#endif

void beego::removeFile(const char *file) {

#if defined(MSC)
  DeleteFileA(file);
#else
  unlink(file);
#endif
}
