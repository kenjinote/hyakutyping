/*!
  \file
  \brief �������̓ǂݏo���ƕύX

  \author Satofumi KAMIMURA

  $Id$

  \todo �]�͂���� umask ��K�؂ɐݒ肷��
  \todo getline() �� eof() �𕹗p���Ȃ��悤�A���̎�����ύX���Ă���
*/

#include "AccessProperty.h"
#include "DetectOS.h"
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <map>
#include <boost/xpressive/xpressive.hpp>
#ifdef WINDOWS_OS
#include <windows.h>
#endif

using namespace beego;
using namespace boost::xpressive;


struct AccessProperty::pImpl {
  std::string error_message_;        //!< �G���[���
  bool read_;                        //!< �ǂݍ��ݍς݂̏ꍇ true
  time_t ctime_;                     //!< �ǂݏo�����̎���
  std::string path_;                 //!< �ǂݏo�����t�@�C���p�X
  typedef std::map<std::string,std::string> PropertyMap;
  PropertyMap properties_;

  pImpl(const char* fname)
    : error_message_("no error."), read_(false), ctime_(0), path_(fname) {
  }

  time_t getChangeTime(void) {

    struct stat buf;
    if (stat(path_.c_str(), &buf) < 0) {
      return 0;
    }
    return buf.st_ctime;
  }

  bool readfile(void) {

    // �ǂݏo������
    // - �擪�� '#' �Ŏn�܂�ꍇ�͓ǂݔ�΂�
    // - �ukey = value�v�ɂ����āAvalue �ȍ~�͓ǂݔ�΂�

    std::ifstream fin(path_.c_str());
    if (! fin.is_open()) {
      return false;
    }

    // �v�f�̓ǂݏo��
    sregex pattern = sregex::compile("^(.+?)\\s*=\\s*\"(.+?)\"\\s*$");
    std::string line;
    while (getline(fin, line)) {
      if (line.empty() || line[0] == '#') {
        continue;
      }

      smatch match;
      if (regex_search(line, match, pattern)) {
        // �i�[
        properties_[match[1]] = match[2];
      }
    }

    read_ = true;
    ctime_ = getChangeTime();
    return true;
  }

  bool save(void) {

    // !!! readfile() �Əd�����������A�d���Ȃ���Ȃ�...

    // �o�̓t�@�C���̐���
#if defined(WINDOWS_OS)
    const char* temp_name = _tempnam(NULL, "access_property_");
#else
    char temp_name[] = "access_property_XXXXXX";
    mkstemp(temp_name);
#endif
    std::ofstream fout(temp_name);

    // �����t�@�C���̓ǂݏo��
    std::ifstream fin(path_.c_str());
    if (! fin.is_open()) {
      return false;
    }

    // �v�f�̓ǂݏo���ƒu��
    sregex pattern = sregex::compile("^(.+?)\\s*=\\s*\"?(.+?)\"?\\s*$");
    std::string line;
    while (getline(fin, line)) {
      smatch match;
      if (regex_search(line, match, pattern)) {

        // �i�[����Ă���L�[���[�h�Ȃ�A�u��������
        const std::string& key = match[1];
        PropertyMap::iterator p = properties_.find(key);
        if (p != properties_.end()) {
          fout << key << " = \"" << p->second << '"' << std::endl;

          // �L�[�̓o�^�폜
          properties_.erase(key);

          continue;
        }
      }
      fout << line << std::endl;
    }

    // ���ۑ��̍��ڂ�ۑ�����
    for (PropertyMap::iterator it = properties_.begin();
         it != properties_.end(); ++it) {
      fout << it->first << " = " << it->second << std::endl;
    }

    // �t�@�C����u������
	fin.close();
    fout.close();
#if defined(WINDOWS_OS)
	// !!! RemoveFile() ���g���ׂ�
	DeleteFileA(path_.c_str());
#endif
	return (rename(temp_name, path_.c_str()) == 0) ? true : false;
  }
};


AccessProperty::AccessProperty(const char* fname) : pimpl(new pImpl(fname)) {
  pimpl->readfile();
}


AccessProperty::~AccessProperty(void) {
}


const char* AccessProperty::what(void) {
  return pimpl->error_message_.c_str();
}


#if 0
void AccessProperty::operator=(const char* value) {
  pimpl->value_ = value;
}


AccessProperty& AccessProperty::operator[](const char* key) {

  pimpl->properties_[key] = pimpl->value_;
  fprintf(stderr, " [%s] = %s\n", key, pimpl->value_.c_str());

  return *this;
}
#endif


std::string& AccessProperty::operator[](const char* key) const {
  return pimpl->properties_[key];
}


bool AccessProperty::reload(void) {
  if (! pimpl->read_) {
    // !!! error_message_ ���X�V
    return false;
  }

  return pimpl->readfile();
}


bool AccessProperty::save(void) {
  if (! pimpl->read_) {
    // !!! error_message_ ���X�V
    return false;
  }

  return pimpl->save();
}


bool AccessProperty::isChangedByOther(void) {
  if (! pimpl->read_) {
    // !!! error_message_ ���X�V
    return false;
  }

  // �ŏI������ǂݏo���A�ǂݍ��񂾎����ƈ���Ă�����Atrue ��Ԃ�
  time_t now_ctime = pimpl->getChangeTime();
  return (now_ctime != pimpl->ctime_) ? true : false;
}
