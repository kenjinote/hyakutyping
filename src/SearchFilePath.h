#ifndef SEARCH_FILE_PATH_H
#define SEARCH_FILE_PATH_H

/*!
  \file
  \brief ファイルのパス探索

  \author Satofumi KAMIMURA

  $Id$
*/

#include <string>
#include <vector>


namespace beego {
  /*!
    \brief 複数パスからのファイルの探索

    \param full_path [o] 探索結果のフルパス
    \param fname [i] ファイル名
    \param dir_path [i] パス文字列の配列

    \retval true ファイル探索に成功、full_path に探索結果のフルパスが格納される
    \retval false ファイル探索に失敗

    使用例
    \code
// ./beegoconf または ~/.beego/beegoconf の探索
std::vector<std::string> search_path;
search_path.push_back("./");
search_path.push_back("~/.beego/");

std::string config_file;
if (searchFilePath(config_file, "beegoconf", search_path)) {
...
} \endcode
  */
  extern bool searchFilePath(std::string& full_path, const char* fname,
                             const std::vector<std::string>& dir_path);
};

#endif /* SEARCH_FILE_PATH_H */
