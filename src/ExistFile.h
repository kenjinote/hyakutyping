#ifndef EXIST_FILE_H
#define EXIST_FILE_H

/*!
  \file
  \brief ファイル有無の確認

  access() のラッパー

  \author Satofumi KAMIMURA
*/

namespace beego {
  extern bool existFile(const char* file_path);
};

#endif /* !EXIST_FILE_H */
