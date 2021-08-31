/*!
  \file
  \brief ディレクトリの作成

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CreateDirectory.h"
#include "ExistFile.h"
#include "DetectOS.h"
#ifdef WINDOWS_OS
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <string>


bool beego::createDirectory(const char* directory) {

  // 末尾の '/' を無視してディレクトリ作成を行う
  std::string dir_path = directory;
  size_t n = dir_path.size();
  if (directory[n - 1] == '/') {
    dir_path = dir_path.substr(0, n - 1);
  }
  std::string next_create = dir_path.substr(0, dir_path.find_last_of('/'));

  // !!! 書き込み権限があるか、を確認すべきかも
  if (existFile(dir_path.c_str())) {
    return true;
  }

  // ディレクトリの作成
#if defined(MinGW)
  return (mkdir(dir_path.c_str()) == 0) ? true : false;
#elif defined(MSC)
  return (_mkdir(dir_path.c_str()) == 0) ? true : false;
#else
  return (mkdir(dir_path.c_str(), 0755) == 0) ? true : false;
#endif

  if (! createDirectory(next_create.c_str())) {
    return false;
  }

  return true;
}
