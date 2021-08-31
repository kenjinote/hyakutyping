#ifndef DRAW_PRIMITIVE_H
#define DRAW_PRIMITIVE_H

/*!
  \file
  \brief ��{�`��

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/shared_ptr.hpp>
#include <SDL.h>


namespace beego {
  class TextSurface;

  extern void draw_registerScrResource(SDL_Surface* scr);

  extern Uint32 draw_getDrawColor(Uint32 color);
  extern void draw_point(int x, int y, Uint32 color);
  extern void draw_line(int x1, int y1, int x2, int y2, Uint32 color);

  // !!! �ǂ������ɂ��܂��傤...
  extern void draw_rect(const SDL_Rect* clear_area, Uint32 color);
  extern void draw_rect(int x1, int y1, int x2, int y2, Uint32 color);

  extern void draw_text(int x, int y, boost::shared_ptr<TextSurface> text);
};

#endif /* !DRAW_PRIMITIVE_H */
