/*!
  \file
  \brief 表示切り替えサーフェス

  \authro Satofumi KAMIMURA

  $Id$
*/

#include "SwitchSurface.h"
#include "SdlUtils.h"
#include "GridTypes.h"

using namespace beego;


struct SwitchSurface::pImpl {
  enum {
    InvalidIndex = -1,
  };
  int now_index;
  bool is_changed;

  class EachSurface {
  public:
    Surface surface;
    Grid<int> offset;
  };
  std::vector<EachSurface> surfaces;

  pImpl(void) : now_index(InvalidIndex), is_changed(false) {
  }
};


SwitchSurface::SwitchSurface(void) : pimpl(new pImpl) {
}


SwitchSurface::~SwitchSurface(void) {
}


void SwitchSurface::draw(std::vector<SDL_Rect>& update_rects,
                         const SDL_Rect* pos, const SDL_Rect* area) {
  if (pimpl->now_index < 0) {
    return;
  }
  int index = pimpl->now_index;
  Surface& p = pimpl->surfaces[index].surface;
  if (p == NULL) {
    fprintf(stderr, "NULL\n");
    return;
  }

  SDL_Rect draw_position;
  set_SdlRect(&draw_position, pos->x + pimpl->surfaces[index].offset.x,
              pos->y + pimpl->surfaces[index].offset.y);
  p->draw(update_rects, &draw_position, area);
}


size_t SwitchSurface::getWidth(void) {
  if (pimpl->now_index < 0) {
    return 0;
  }

  Surface& p = pimpl->surfaces[pimpl->now_index].surface;
  if (p == NULL) {
    return 0;
  }
  return p->getWidth();
}


size_t SwitchSurface::getHeight(void) {
  if (pimpl->now_index < 0) {
    return 0;
  }

  Surface& p = pimpl->surfaces[pimpl->now_index].surface;
  if (p == NULL) {
    return 0;
  }
  return p->getHeight();
}


void SwitchSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool SwitchSurface::isChanged(size_t ticks) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool SwitchSurface::isTransparent(void) {
  if (pimpl->now_index < 0) {
    return true;
  }
  return pimpl->surfaces[pimpl->now_index].surface->isTransparent();
}


void SwitchSurface::clear(void) {
  pimpl->surfaces.clear();
}


bool SwitchSurface::empty(void) {
  return pimpl->surfaces.empty();
}


size_t SwitchSurface::size(void) {
  return pimpl->surfaces.size();
}


void SwitchSurface::registerSurface(Surface surface,
                                    int x_offset, int y_offset, size_t index) {

  // 指定 index 分の領域を確保
  if (index >= pimpl->surfaces.size()) {
    pimpl->surfaces.resize(index + 1);
  }

  // サーフェスを登録
  std::swap(pimpl->surfaces[index].surface, surface);
  pimpl->surfaces[index].offset.x = x_offset;
  pimpl->surfaces[index].offset.y = y_offset;

  // サーフェスが変更されたかの判定
  if (static_cast<int>(index) == pimpl->now_index) {
    pimpl->is_changed = true;
  }
}


void SwitchSurface::switchSurface(size_t index) {
  if (index < pimpl->surfaces.size()) {
    if (pimpl->now_index != static_cast<int>(index)) {
      pimpl->now_index = static_cast<int>(index);
      pimpl->is_changed = true;
    }
  }
}


size_t SwitchSurface::getSurfaceIndex(void) {
  return pimpl->now_index;
}


Surface SwitchSurface::getSurface(size_t index) {
  return pimpl->surfaces[index].surface;
}
