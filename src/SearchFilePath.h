#ifndef SEARCH_FILE_PATH_H
#define SEARCH_FILE_PATH_H

/*!
  \file
  \brief �t�@�C���̃p�X�T��

  \author Satofumi KAMIMURA

  $Id$
*/

#include <string>
#include <vector>


namespace beego {
  /*!
    \brief �����p�X����̃t�@�C���̒T��

    \param full_path [o] �T�����ʂ̃t���p�X
    \param fname [i] �t�@�C����
    \param dir_path [i] �p�X������̔z��

    \retval true �t�@�C���T���ɐ����Afull_path �ɒT�����ʂ̃t���p�X���i�[�����
    \retval false �t�@�C���T���Ɏ��s

    �g�p��
    \code
// ./beegoconf �܂��� ~/.beego/beegoconf �̒T��
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
