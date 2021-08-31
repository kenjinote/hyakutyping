#ifndef SDL_SURFACE_H
#define SDL_SURFACE_H

/*!
  \file
  \brief SDL サーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  /*!
    \brief SDL サーフェスの描画
  */
  class SdlSurface : public SurfaceInterface {
    SdlSurface(void);
    SdlSurface(const SdlSurface& rhs);
    SdlSurface& operator = (const SdlSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit SdlSurface(SDL_Surface* surface, bool transparent = false);
    ~SdlSurface(void);

    bool isInvalid(void);
    void transparent(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void setAlpha(size_t percent);
    SDL_Surface* getRawSurface(void);
  };
};

#endif /* !SDL_SURFACE_H */
