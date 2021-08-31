/*!
  \file
  \brief �t�@�C���T��

  \author Satofumi KAMIMURA

  $Id$

  \todo �����́AMSC �p�Ƃ����łȂ��p�Ɋ��S�ɕ�����B�ǂ݂ɂ���
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
  \brief recursive �͖������B�����A�������Ȃ�...
*/
size_t beego::findFiles(std::vector<std::string>& files,
                        const char* findRootPath,
                        boost::xpressive::sregex pattern, bool recursive) {

  // �f�B���N�g������t�@�C���ꗗ���擾����
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
    // !!! �G���[�o��
    return 0;
  }
#endif
  size_t n = strlen(findRootPath);
  const char* add_str = (findRootPath[n -1] == '/') ? "" : "/";

  // �t�@�C�����Ƀp�^�[���Ƀ}�b�`���邩�̔�����s��
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
