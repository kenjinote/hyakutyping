/*!
  \file
  \brief �z�񉻂����t�@�C���̃f�R�[�h

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

  // !!! �ǂ�����āAstatic_cast ��K�p����񂾁H
  fout.write((const char*)array, count);
  return true;
}
