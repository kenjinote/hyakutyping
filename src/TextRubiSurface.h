#ifndef TEXT_RUBI_SURFACE_H
#define TEXT_RUBI_SURFACE_H

/*!
  \file
  \brief 文字列のルビサーフェス作成

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include "FadeSurfacesInterface.h"
#include "GuiColors.h"
#include <memory>

namespace beego {
  class TtfResource;
  class TextProperty;

  class TextRubiSurface : public SurfaceInterface {
    TextRubiSurface(void);
    TextRubiSurface(const TextRubiSurface& rhs);
    TextRubiSurface& operator = (const TextRubiSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    TextRubiSurface(const Uint16* kana_text, size_t rubi_size,
                    const TextProperty& property);
    ~TextRubiSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void setFadeMode(FadeMode mode, size_t width);
    void setFadePercent(size_t percent);
    int getDrawOffset(void);
  };
};

#endif /* !TEXT_RUBI_SURFACE_H */
