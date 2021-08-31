#ifndef UTF_STRING_H
#define UTF_STRING_H

/*!
  \file
  \brief UTF8 ‚Ì•¶š—ñ‘€ìŠÖ”

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <stddef.h>
#include <vector>


namespace beego {
  extern size_t ustrlen(const Uint16* str);
  extern int ustrncmp(const Uint16* a, const Uint16* b, size_t n);
  extern void ustrcat(std::vector<Uint16>& dest, const char* src);
  extern void ustrcat(std::vector<Uint16>& dest, const Uint16* src);
  extern bool isHiragana(const Uint16 wch);
  extern void uni2char(std::vector<char>& text, std::vector<Uint16>& utext);
};

#endif /* !UTF_STRING_H */
