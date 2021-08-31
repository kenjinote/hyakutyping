#ifndef REDRAW_SURFACE_H
#define REDRAW_SURFACE_H

/*!
  \file
  \brief 再描画のためのサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  class RedrawSurface : public SurfaceInterface {
    RedrawSurface(const RedrawSurface& rhs);
    RedrawSurface& operator = (const RedrawSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    RedrawSurface(void);
    ~RedrawSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void setRedrawRect(SDL_Rect& rect);
  };
};

#endif /* !REDRAW_SURFACE_H */
