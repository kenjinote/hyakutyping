/*!
  \file
  \brief 単一色サーフェス

  \author Satofumi KAMIMURA

  $Id$

  \todo α値を変更するメソッドを追加する
*/

#include "ColorSurface.h"
#include "LogManager.h"
#include "SdlUtils.h"
#include "GuiColors.h"
#include <string>

using namespace beego;


struct ColorSurface::pImpl {
  SDL_Surface* scr;
  Uint32 surface_color;
  size_t width;
  size_t height;
  bool is_changed;

  pImpl(Uint32 color, size_t w, size_t h)
    : scr(SDL_GetVideoSurface()), width(w), height(h), is_changed(false) {
    if (scr == NULL) {
      LogManager* log = LogManager::getObject();
      std::string message = "SdlSurface::pImpl(): "
        "Screen surface must be created.";
      log->write(LogManager::Warning, message.c_str());
      return;
    }
    surface_color = getSdlColor(scr, color);
  }
};


ColorSurface::ColorSurface(size_t width, size_t height, Uint32 color)
  : pimpl(new pImpl(color, width, height)) {
}


ColorSurface::~ColorSurface(void) {
}

void ColorSurface::draw(std::vector<SDL_Rect>& update_rects,
                        const SDL_Rect* pos, const SDL_Rect* area) {

  SDL_Rect draw_rect;
  set_SdlRect(&draw_rect, pos->x, pos->y, area->w, area->h);

  SDL_FillRect(pimpl->scr, &draw_rect, pimpl->surface_color);
  update_rects.push_back(draw_rect);
}


size_t ColorSurface::getWidth(void) {
  return pimpl->width;
}


size_t ColorSurface::getHeight(void) {
  return pimpl->height;
}


void ColorSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool ColorSurface::isChanged(size_t ticks) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool ColorSurface::isTransparent(void) {
  return false;
}
