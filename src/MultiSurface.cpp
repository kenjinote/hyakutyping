/*!
  \file
  \brief 重ね合わせサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "MultiSurface.h"
#include "GridTypes.h"
#include "SdlUtils.h"

using namespace beego;


struct MultiSurface::pImpl {
  size_t width_;
  size_t height_;
  bool is_transparent_;
  std::vector<Grid<int> > offset_;

  std::vector<Surface> surfaces_;

  pImpl(size_t width, size_t height)
    : width_(width), height_(height), is_transparent_(false) {
  }
};


MultiSurface::MultiSurface(size_t width, size_t height)
  : pimpl(new pImpl(width, height)) {
}


MultiSurface::~MultiSurface(void) {
}


void MultiSurface::draw(std::vector<SDL_Rect>& update_rects,
                        const SDL_Rect* pos, const SDL_Rect* area) {

  std::vector<Grid<int> >::reverse_iterator offset_it = pimpl->offset_.rbegin();
  for (std::vector<Surface>::reverse_iterator it = pimpl->surfaces_.rbegin();
       it != pimpl->surfaces_.rend(); ++it, ++offset_it) {

    SDL_Rect draw_position;
    set_SdlRect(&draw_position, pos->x + offset_it->x, pos->y + offset_it->y);
    (*it)->draw(update_rects, &draw_position, area);
  }
}


size_t MultiSurface::getWidth(void) {
  return pimpl->width_;
}


size_t MultiSurface::getHeight(void) {
  return pimpl->height_;
}


void MultiSurface::forceSetChanged(void) {

  // !!! まぁ、全サーフェスに対して適用する必要はないはずなんだが、一応
  for (std::vector<Surface>::iterator it = pimpl->surfaces_.begin();
       it != pimpl->surfaces_.end(); ++it) {
    (*it)->forceSetChanged();
  }
}


bool MultiSurface::isChanged(size_t ticks) {

  bool is_changed = false;
  for (std::vector<Surface>::iterator it = pimpl->surfaces_.begin();
       it != pimpl->surfaces_.end(); ++it) {
    is_changed |= (*it)->isChanged(ticks);
  }
  return is_changed;
}


bool MultiSurface::isTransparent(void) {
  return pimpl->is_transparent_;
}


void MultiSurface::registerSurface(Surface surface,
                                   int x_offset, int y_offset) {

  pimpl->is_transparent_ |= surface->isTransparent();
  pimpl->surfaces_.push_back(surface);
  pimpl->offset_.push_back(Grid<int>(x_offset, y_offset));
}
