#ifndef MULTI_SURFACE_H
#define MULTI_SURFACE_H

/*!
  \file
  \brief 重ね合わせサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include "Surface.h"
#include <memory>


namespace beego {
  class MultiSurface : public SurfaceInterface {
    MultiSurface(void);
    MultiSurface(const MultiSurface& rhs);
    MultiSurface& operator = (const MultiSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    MultiSurface(size_t width, size_t height);
    ~MultiSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void registerSurface(Surface surface, int x_offset = 0, int y_offset = 0);
  };
};

#endif /* !MULTI_SURFACE_H */
