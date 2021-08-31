#ifndef COLOR_SURFACE_H
#define COLOR_SURFACE_H

/*!
  \file
  \brief �P��F�T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  \todo �A���t�@�l���ςɂł���悤�ɕύX����
*/

#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  class ColorSurface : public SurfaceInterface {
    ColorSurface(void);
    ColorSurface(const ColorSurface& rhs);
    ColorSurface& operator = (const ColorSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit ColorSurface(size_t width, size_t height, Uint32 color);
    ~ColorSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);
  };
};

#endif /* !COLOR_SURFACE_H */
