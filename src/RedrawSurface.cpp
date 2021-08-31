/*!
  \file
  \brief 再描画のためのサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "RedrawSurface.h"

using namespace beego;


struct RedrawSurface::pImpl {
  SDL_Rect redraw_rect_;
};


RedrawSurface::RedrawSurface(void) : pimpl(new pImpl) {
}


RedrawSurface::~RedrawSurface(void) {
}


void RedrawSurface::draw(std::vector<SDL_Rect>& update_rects,
                         const SDL_Rect* pos, const SDL_Rect* area) {

  // 実際の描画は行わない
}


size_t RedrawSurface::getWidth(void) {
  return pimpl->redraw_rect_.w;
}


size_t RedrawSurface::getHeight(void) {
  return pimpl->redraw_rect_.h;
}


void RedrawSurface::forceSetChanged(void) {
  // 常に isChanged が true を返すため、何もしなくてよい
}


bool RedrawSurface::isChanged(size_t ticks) {
  return true;
}


bool RedrawSurface::isTransparent(void) {
  return true;
}


void RedrawSurface::setRedrawRect(SDL_Rect& rect) {
  pimpl->redraw_rect_ = rect;
}
