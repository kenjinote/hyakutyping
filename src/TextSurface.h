#ifndef TEXT_SURFACE_H
#define TEXT_SURFACE_H

/*!
  \file
  \brief 文字サーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include "GuiColors.h"
#include <memory>

namespace beego {
  class TtfResource;
  class TextProperty;

  /*!
    \brief 文字サーフェス
  */
  class TextSurface : public SurfaceInterface {
    TextSurface(const TextSurface& rhs);
    TextSurface& operator = (const TextSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    TextSurface(const TextProperty& text_property);
    ~TextSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    // !!! いっそのこと、SurfaceInterface に追加するべきか？
    void setAlpha(size_t percent);
  };
};

#endif /* !TEXT_SURFACE_H */
