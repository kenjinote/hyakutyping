/*!
  \file
  \brief ファイル探索

  \author Satofumi KAMIMURA

  $Id$

  \todo 実装は、MSC 用とそうでない用に完全に分ける。読みにくい
*/

#include "FindFiles.h"
#include <sys/types.h>
#include "DetectOS.h"
#ifdef MSC
#include <windows.h>
#else
#include <dirent.h>
#endif

using namespace boost::xpressive;


/*!
  \brief recursive は未実装。多分、実装しない...
*/
size_t beego::findFiles(std::vector<std::string>& files,
                        const char* findRootPath,
                        boost::xpressive::sregex pattern, bool recursive) {

  // ディレクトリからファイル一覧を取得する
#ifdef MSC
  WIN32_FIND_DATAA fd;
  std::string match_pattern = std::string(findRootPath) + "*";
  HANDLE dir = FindFirstFileA(match_pattern.c_str(), &fd);
  if (dir == INVALID_HANDLE_VALUE) {
    return 0;
  }
#else
  DIR* dir = opendir(findRootPath);
  if (dir == NULL) {
    // !!! エラー出力
    return 0;
  }
#endif
  size_t n = strlen(findRootPath);
  const char* add_str = (findRootPath[n -1] == '/') ? "" : "/";

  // ファイル毎にパターンにマッチするかの判定を行う
  size_t found_num = 0;
#ifdef MSC
  do {
    std::string line = fd.cFileName;
#else
  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    std::string line = entry->d_name;
#endif
    smatch match;
    if (regex_search(line, match, pattern)) {
      files.push_back(std::string(findRootPath) + add_str + line);
      ++found_num;
    }
#ifdef MSC
  } while (FindNextFileA(dir, &fd));
  FindClose(dir);
#else
  }
  closedir(dir);
#endif
  return found_num;
}
