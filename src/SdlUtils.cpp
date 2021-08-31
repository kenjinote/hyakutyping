/*!
  \file
  \brief SDL •Ï”‚Ì‘€ì•â•

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlUtils.h"


void beego::set_SdlRect(SDL_Rect* rect, int x, int y, int w, int h) {
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
}


void beego::add_SdlRect(std::vector<SDL_Rect>& list,
                        int x, int y, int w, int h) {
  SDL_Rect rect;
  set_SdlRect(&rect, x, y, w, h);
  list.push_back(rect);
}


void beego::set_SdlColor(SDL_Color* color, Uint32 uint32_color) {
  color->r = (uint32_color >> 24) & 0xff;
  color->g = (uint32_color >> 16) & 0xff;
  color->b = (uint32_color >> 8) & 0xff;
}
