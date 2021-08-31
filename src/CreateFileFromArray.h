#ifndef CREATE_FILE_FROM_ARRAY_H
#define CREATE_FILE_FROM_ARRAY_H

/*!
  \file
  \brief 配列化したファイルのデコード

  \author Satofumi KAMIMURA

  $Id$
*/

#include <stddef.h>


namespace beego {
  extern bool createFileFromArray(const char* fname,
                                  const unsigned char* array, size_t count);
};

#endif /* !CREATE_FILE_FROM_ARRAY_H */
