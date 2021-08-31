/*!
  \file
  \brief 属性情報の読み出しと変更

  \author Satofumi KAMIMURA

  $Id$

  \todo 余力あらば umask を適切に設定する
  \todo getline() と eof() を併用しないよう、他の実装を変更していく
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
  std::string error_message_;        //!< エラー状態
  bool read_;                        //!< 読み込み済みの場合 true
  time_t ctime_;                     //!< 読み出し時の時刻
  std::string path_;                 //!< 読み出したファイルパス
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

    // 読み出し処理
    // - 先頭が '#' で始まる場合は読み飛ばす
    // - 「key = value」において、value 以降は読み飛ばす

    std::ifstream fin(path_.c_str());
    if (! fin.is_open()) {
      return false;
    }

    // 要素の読み出し
    sregex pattern = sregex::compile("^(.+?)\\s*=\\s*\"(.+?)\"\\s*$");
    std::string line;
    while (getline(fin, line)) {
      if (line.empty() || line[0] == '#') {
        continue;
      }

      smatch match;
      if (regex_search(line, match, pattern)) {
        // 格納
        properties_[match[1]] = match[2];
      }
    }

    read_ = true;
    ctime_ = getChangeTime();
    return true;
  }

  bool save(void) {

    // !!! readfile() と重複が多いが、仕方ないよなぁ...

    // 出力ファイルの生成
#if defined(WINDOWS_OS)
    const char* temp_name = _tempnam(NULL, "access_property_");
#else
    char temp_name[] = "access_property_XXXXXX";
    mkstemp(temp_name);
#endif
    std::ofstream fout(temp_name);

    // 既存ファイルの読み出し
    std::ifstream fin(path_.c_str());
    if (! fin.is_open()) {
      return false;
    }

    // 要素の読み出しと置換
    sregex pattern = sregex::compile("^(.+?)\\s*=\\s*\"?(.+?)\"?\\s*$");
    std::string line;
    while (getline(fin, line)) {
      smatch match;
      if (regex_search(line, match, pattern)) {

        // 格納されているキーワードなら、置き換える
        const std::string& key = match[1];
        PropertyMap::iterator p = properties_.find(key);
        if (p != properties_.end()) {
          fout << key << " = \"" << p->second << '"' << std::endl;

          // キーの登録削除
          properties_.erase(key);

          continue;
        }
      }
      fout << line << std::endl;
    }

    // 未保存の項目を保存する
    for (PropertyMap::iterator it = properties_.begin();
         it != properties_.end(); ++it) {
      fout << it->first << " = " << it->second << std::endl;
    }

    // ファイルを置換する
	fin.close();
    fout.close();
#if defined(WINDOWS_OS)
	// !!! RemoveFile() を使うべき
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
    // !!! error_message_ を更新
    return false;
  }

  return pimpl->readfile();
}


bool AccessProperty::save(void) {
  if (! pimpl->read_) {
    // !!! error_message_ を更新
    return false;
  }

  return pimpl->save();
}


bool AccessProperty::isChangedByOther(void) {
  if (! pimpl->read_) {
    // !!! error_message_ を更新
    return false;
  }

  // 最終時刻を読み出し、読み込んだ時刻と違っていたら、true を返す
  time_t now_ctime = pimpl->getChangeTime();
  return (now_ctime != pimpl->ctime_) ? true : false;
}
