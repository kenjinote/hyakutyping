/*!
  \file
  \brief 空サーフェス

  \author Satofumi KAMIMURA

  $Id$

  Surface に NULL を指定する代わりにこのサーフェスを使う
*/

#include "NullSurface.h"

using namespace beego;


NullSurface::NullSurface(void) {
}


NullSurface::~NullSurface(void) {
}


void NullSurface::draw(std::vector<SDL_Rect>& update_rects,
                       const SDL_Rect* pos, const SDL_Rect* area) {
  // 何もしない
}


size_t NullSurface::getWidth(void) {
  return 0;
}


size_t NullSurface::getHeight(void) {
  return 0;
}


void NullSurface::forceSetChanged(void) {
  // 何もしない
}


bool NullSurface::isChanged(size_t ticks) {
  return false;
}


bool NullSurface::isTransparent(void) {
  return false;
}
