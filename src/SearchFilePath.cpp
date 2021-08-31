/*!
  \file
  \brief ファイルのパス探索

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SearchFilePath.h"
#include "ExistFile.h"
#include <cstdlib>
#include <cstring>

using namespace beego;


bool beego::searchFilePath(std::string& full_path, const char* fname,
                           const std::vector<std::string>& dir_path) {

  // ファイルの存在を順に確認する
  std::vector<std::string> search_dir = dir_path;
  if (search_dir.empty()) {
    search_dir.push_back(".");
  }
  for (std::vector<std::string>::const_iterator it = search_dir.begin();
       it != search_dir.end(); ++it) {

    // "~/" を環境変数 HOME の内容に置換する
    std::string replaced = *it;

    size_t match_index = it->find("~/");
    if (match_index != std::string::npos) {
      const char* home = getenv("HOME");
      if (home != NULL) {
        size_t n = strlen(home);
        replaced.replace(match_index, 1, home, n);
      }
    }
    std::string try_path = replaced + "/" + fname;
    if (existFile(try_path.c_str())) {
      full_path = try_path;
      return true;
    }
  }
  return false;
}
