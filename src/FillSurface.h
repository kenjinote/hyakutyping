#ifndef FILL_SURFACE_H
#define FILL_SURFACE_H

/*!
  \file
  \brief 敷き詰めサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Surface.h"
#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  /*!
    \brief 塗りつぶしサーフェス処理クラス

    \attention サーフェス処理のためには画面サーフェス生成が必要

    \todo pos, area の関係を修正する
  */
  class FillSurface : public SurfaceInterface {
    FillSurface(const FillSurface& rhs);
    FillSurface& operator = (const FillSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit FillSurface(Surface surface, size_t width, size_t height,
                         size_t x_offset = 0, size_t y_offset = 0);
    ~FillSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);
  };
};

#endif /* !FILL_SURFACE_H */
