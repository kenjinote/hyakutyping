/*!
  \file
  \brief Šî–{•`‰æ

  \author Satofumi KAMIMURA

  $Id$
*/

#include "DrawPrimitive.h"
#include "SdlUtils.h"
#include "GuiColors.h"
#include "TextSurface.h"


static SDL_Surface* Scr = NULL;


void beego::draw_registerScrResource(SDL_Surface* scr) {
  Scr = scr;
}


Uint32 beego::draw_getDrawColor(Uint32 color) {
  if (! Scr) {
    return color;
  }
  return getSdlColor(Scr, color);
}


void beego::draw_point(int x, int y, Uint32 color) {
  if (! Scr) {
    return;
  }

  SDL_Rect draw_point;
  set_SdlRect(&draw_point, x, y, 1, 1);
  SDL_FillRect(Scr, &draw_point, color);
}


void beego::draw_line(int x1, int y1, int x2, int y2, Uint32 color) {

  int width = abs(x2 - x1);
  int height = abs(y2 - y1);
  int yy = (y1 < y2) ? +1 : -1;
  int xx = (x1 < x2) ? +1 : -1;

  if (width >= height) {
    int y = y1;
    int fraction = width;
    for (int x = x1; x != x2; x += xx) {
      draw_point(x, y, color);
      fraction += (height << 1);
      if (fraction >= (width << 1)) {
        fraction -= (width << 1);
        y += yy;
      }
    }

  } else {
    int x = x1;
    int fraction = height;
    for (int y = y1; y != y2; y += yy) {
      draw_point(x, y, color);
      fraction += (width << 1);
      if (fraction >= (height << 1)) {
        fraction -= (height << 1);
        x += xx;
      }
    }
  }
}


void beego::draw_rect(const SDL_Rect* clear_area, Uint32 color) {
  if (! Scr) {
    return;
  }
  SDL_FillRect(Scr, const_cast<SDL_Rect*>(clear_area), color);
}



void beego::draw_rect(int x, int y, int w, int h, Uint32 color) {
  if (! Scr) {
    return;
  }

  SDL_Rect draw_rect;
  set_SdlRect(&draw_rect, x, y, w, h);
  SDL_FillRect(Scr, &draw_rect, color);
}


void beego::draw_text(int x, int y, boost::shared_ptr<TextSurface> text) {

  size_t w = text->getWidth();
  size_t h = text->getHeight();
  SDL_Rect pos;
  set_SdlRect(&pos, x, y, w, h);

  SDL_Rect area;
  set_SdlRect(&area, 0, 0, w, h);

  std::vector<SDL_Rect> dummy;
  text->draw(dummy, &pos, &area);
}
