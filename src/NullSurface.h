#ifndef NULL_SURFACE_H
#define NULL_SURFACE_H

/*!
  \file
  \brief 空サーフェス

  \author Satofumi KAMIMURA

  $Id$

  Surface に NULL を指定する代わりにこのサーフェスを使う
*/

#include "SurfaceInterface.h"


namespace beego {
  class NullSurface : public SurfaceInterface {
    NullSurface(const NullSurface& rhs);
    NullSurface& operator = (const NullSurface& rhs);

  public:
    NullSurface(void);
    ~NullSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);
  };
};

#endif /* !NULL_SURFACE_H */
