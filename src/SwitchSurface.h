#ifndef SWITCH_SURFACE_H
#define SWITCH_SURFACE_H

/*!
  \file
  \brief 表示切り替えサーフェス

  \authro Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include "Surface.h"
#include <memory>


namespace beego {
  class SwitchSurface : public SurfaceInterface {
    SwitchSurface(const SwitchSurface& rhs);
    SwitchSurface& operator = (const SwitchSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    SwitchSurface(void);
    ~SwitchSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void clear(void);
    bool empty(void);
    size_t size(void);
    void registerSurface(Surface surface,
                         int x_offset, int y_offset, size_t index);
    void switchSurface(size_t index);
    size_t getSurfaceIndex(void);
    Surface getSurface(size_t index);
  };
};

#endif /* !SWITCH_SURFACE_H */
