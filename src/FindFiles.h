#ifndef FIND_FILES_H
#define FIND_FILES_H

/*!
  \file
  \brief ファイル探索

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <vector>


namespace beego {

  /*!
    \brief ファイル探索を行う

    \param files [o] マッチしたファイルの追加先
    \param findRootPath [i] 探索を開始するディレクトリ
    \param filePattern [i] 探索するファイルの正規表現
    \param recursive [i] 再帰的に探索するか

    \return マッチしたファイル数
  */
  size_t findFiles(std::vector<std::string>& files, const char* findRootPath,
                   boost::xpressive::sregex pattern, bool recursive = false);
};

#endif /* !FIND_FILES_H */
