#ifndef TEXT_FADE_SURFACE_H
#define TEXT_FADE_SURFACE_H

/*!
  \file
  \brief フェード可能なテキストサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include "FadeSurfacesInterface.h"
#include "GuiColors.h"
#include <memory>

namespace beego {
  class TtfResource;
  class FadeSurfacesInterface;
  class TextProperty;

  class TextFadeSurface : public SurfaceInterface {
    TextFadeSurface(void);
    TextFadeSurface(const TextFadeSurface& rhs);
    TextFadeSurface& operator = (const TextFadeSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    TextFadeSurface(const TextProperty& property);
    TextFadeSurface(FadeSurfacesInterface* fade, size_t fade_width);
    ~TextFadeSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void setFadeMode(FadeMode mode, size_t width);
    void setFadePercent(size_t percent);
  };
};

#endif /* !TEXT_FADE_SURFACE_H */
