/*!
  \file
  \brief UTF8 の文字列操作関数

  \author Satofumi KAMIMURA

  $Id$

  \todo 関数の構成とオーバーロードのあたりを再検討する
*/

#include "UtfString.h"


size_t beego::ustrlen(const Uint16* str) {

  int length = 0;
  for (; str[length] != 0x0; ++length)
    ;

  return length;
}


int beego::ustrncmp(const Uint16* a, const Uint16* b, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    int cmp = a[i] - b[i];
    if (cmp != 0) {
      return cmp;
    }
  }
  return 0;
}


void beego::ustrcat(std::vector<Uint16>& dest, const char* src) {
  if ((! dest.empty()) && (dest.back() == 0x0)) {
    dest.pop_back();
  }

  size_t n = strlen(src);
  for (size_t i = 0; i < n; ++i) {
    dest.push_back(src[i]);
  }
  dest.push_back(0x0);
}


void beego::ustrcat(std::vector<Uint16>& dest, const Uint16* src) {
  if ((! dest.empty()) && (dest.back() == 0x0)) {
    dest.pop_back();
  }

  // !!! ustrlen(const char*) になれば、ustrcat() の中身は、１つで済むのかな？
  size_t n = ustrlen(src);
  for (size_t i = 0; i < n; ++i) {
    dest.push_back(src[i]);
  }
  dest.push_back(0x0);
}


bool beego::isHiragana(const Uint16 wch) {
  if ((wch == 0) || (wch == 0x3090) || (wch == 0x3091) || (wch > 0x30f6)) {
    return false;
  } else {
    return true;
  }
}


void beego::uni2char(std::vector<char>& text, std::vector<Uint16>& utext) {

  for (std::vector<Uint16>::iterator it = utext.begin();
       it != utext.end(); ++it) {
    text.push_back(0xff & *it);
  }
}
