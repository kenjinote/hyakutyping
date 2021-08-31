/*!
  \file
  \brief 配列化したファイルのデコード

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CreateFileFromArray.h"
#include <fstream>


bool beego::createFileFromArray(const char* fname,
                                const unsigned char* array, size_t count) {

  std::ofstream fout(fname, std::ios::binary);
  if (! fout.is_open()) {
    return false;
  }

  // !!! どうやって、static_cast を適用するんだ？
  fout.write((const char*)array, count);
  return true;
}
