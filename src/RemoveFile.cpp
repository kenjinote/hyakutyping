/*!
  \file
  \brief �t�@�C���폜

  \author Satofumi KAMIMURA

  $Id$

  \todo MSC �p�̒������s��
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
