#ifndef FIND_FILES_H
#define FIND_FILES_H

/*!
  \file
  \brief �t�@�C���T��

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <vector>


namespace beego {

  /*!
    \brief �t�@�C���T�����s��

    \param files [o] �}�b�`�����t�@�C���̒ǉ���
    \param findRootPath [i] �T�����J�n����f�B���N�g��
    \param filePattern [i] �T������t�@�C���̐��K�\��
    \param recursive [i] �ċA�I�ɒT�����邩

    \return �}�b�`�����t�@�C����
  */
  size_t findFiles(std::vector<std::string>& files, const char* findRootPath,
                   boost::xpressive::sregex pattern, bool recursive = false);
};

#endif /* !FIND_FILES_H */
