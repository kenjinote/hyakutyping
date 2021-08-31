/*!
  \file
  \brief フェード可能なテキストサーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TextFadeSurface.h"
#include "TextProperty.h"
#include "TextSurface.h"

using namespace beego;


struct TextFadeSurface::pImpl {

  class NormalTextFade : public FadeSurfacesInterface {
    FadeSurfaces ch_surfaces;
    std::vector<int> ch_positions;
    size_t width;
    size_t height;

    void pushSurface(Uint16 ch, TtfResource* font, size_t size,
                     Uint32 fore, Uint32 back, bool transparent) {
      Uint16 text[2] = { 0x0, 0x0 };
      text[0] = ch;
      TextProperty text_property(font, text, size, fore, back, transparent);
      boost::shared_ptr<TextSurface> surface(new TextSurface(text_property));
      ch_positions.push_back(width);
      width += surface->getWidth();
      size_t surface_height = surface->getHeight();
      if (height < surface_height) {
        height = surface_height;
      }
      ch_surfaces.push_back(surface);
    }

  public:
    NormalTextFade(const TextProperty& property) : width(0), height(0) {

      // !!! 見苦しい。置き換え方を思いついたら、置き換える
      // !!! とりあえず、これで
      TtfResource* font = property.font;
      size_t size = property.size;
      Uint32 fore = property.fore_color;
      Uint32 back = property.back_color;
      bool transparent = property.transparent;
      if (property.text) {
        for (int i = 0; property.text[i] != '\0'; ++i) {
          pushSurface(property.text[i], font, size, fore, back, transparent);
        }
      } else {
        for (int i = 0; property.utext[i] != '\0'; ++i) {
          pushSurface(property.utext[i], font, size, fore, back, transparent);
        }
      }
    }

    ~NormalTextFade(void) {
    }

    std::vector<boost::shared_ptr<TextSurface> >& getSurfaces(void) {
      return ch_surfaces;
    }

    int getPositionPercent(size_t index) {
      return (100 * ch_positions[index] / width);
    }

    size_t getWidth(void) {
      return width;
    }

    size_t getHeight(void) {
      return height;
    }
  };

  enum {
    DefaultFontNum = 3,
  };
  FadeMode fade_mode;
  NormalTextFade* normal_fade;
  FadeSurfacesInterface* fade_obj;
  std::vector<int> surfaces_alpha;
  bool is_changed;
  size_t fade_width;
  size_t now_percent;

  pImpl(const TextProperty& property)
    : fade_mode(FadeInFromLeft), normal_fade(new NormalTextFade(property)),
      fade_obj(normal_fade), is_changed(false), now_percent(100) {
    setFadeMode(fade_mode, property.size * DefaultFontNum);
    initAlphaValue();
  }

  pImpl(FadeSurfacesInterface* fade, size_t fade_width)
    : fade_mode(FadeInFromLeft), normal_fade(NULL),
      fade_obj(fade), is_changed(false), now_percent(100) {

    setFadeMode(fade_mode, fade_width);
    initAlphaValue();
  }

  ~pImpl(void) {
    delete normal_fade;
  }

  void setFadeMode(FadeMode mode, size_t w) {
    fade_mode = mode;
    size_t fade_surface_width = fade_obj->getWidth();
    if (fade_surface_width > 0) {
      fade_width = 100 * w / fade_surface_width;
    } else {
      fade_width = 0;
    }
  }

  void initAlphaValue(void) {
    FadeSurfaces& surfaces = fade_obj->getSurfaces();
    size_t n = surfaces.size();
    surfaces_alpha.assign(n, 100);
  }
};


TextFadeSurface::TextFadeSurface(const TextProperty& property)
  : pimpl(new pImpl(property)) {
}


TextFadeSurface::TextFadeSurface(FadeSurfacesInterface* fade,
                                 size_t fade_width)
  : pimpl(new pImpl(fade, fade_width)) {
}


TextFadeSurface::~TextFadeSurface(void) {
}


void TextFadeSurface::draw(std::vector<SDL_Rect>& update_rects,
                           const SDL_Rect* pos, const SDL_Rect* area) {
  if (pimpl->fade_width == 0) {
    // サーフェスがない場合
    return;
  }

  FadeSurfaces& surfaces = pimpl->fade_obj->getSurfaces();

  int index = 0;
  int total_size = getWidth();
  for (FadeSurfaces::iterator it = surfaces.begin();
       it != surfaces.end(); ++it, ++index) {
    SDL_Rect ch_pos = *pos;
    ch_pos.w = (*it)->getWidth();
    ch_pos.h = (*it)->getHeight();
    ch_pos.x += total_size * pimpl->fade_obj->getPositionPercent(index) / 100;
    (*it)->draw(update_rects, &ch_pos, area);
  }
}


size_t TextFadeSurface::getWidth(void) {
  return pimpl->fade_obj->getWidth();
}


size_t TextFadeSurface::getHeight(void) {
  return pimpl->fade_obj->getHeight();
}


void TextFadeSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool TextFadeSurface::isChanged(size_t ticks) {
  if (pimpl->fade_width == 0) {
    // サーフェスがない場合は、常に変更なしにする
    return false;
  }
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool TextFadeSurface::isTransparent(void) {
  if (pimpl->fade_width == 0) {
    // サーフェスがない場合は、常に透過扱いにする
    return true;
  }
  return (pimpl->now_percent == 0) ? false : true;
}


void TextFadeSurface::setFadeMode(FadeMode mode, size_t width) {
  pimpl->setFadeMode(mode, width);
}


void TextFadeSurface::setFadePercent(size_t percent) {
  if (pimpl->fade_width == 0) {
    // サーフェスがない場合
    return;
  }

  percent = (percent > 100) ? 100 : percent;
  if (pimpl->now_percent == percent) {
    return;
  }
  pimpl->now_percent = percent;
  FadeSurfaces& surfaces = pimpl->fade_obj->getSurfaces();

  // 各文字のアルファ値を計算し、描画時に反映させる
  if (pimpl->fade_mode == FadeInFromLeft) {

    size_t index = 0;
    for (FadeSurfaces::iterator it = surfaces.begin();
         it != surfaces.end(); ++it, ++index) {
      int ch_pos = pimpl->fade_obj->getPositionPercent(index);
      int fadeIn_pos =
        ((100 + pimpl->fade_width) * percent / 100) - pimpl->fade_width;
      int fadeOut_pos = fadeIn_pos + pimpl->fade_width;

      int alpha = 0;
      if (ch_pos <= fadeIn_pos) {
        alpha = 100;

      } else if (ch_pos < fadeOut_pos) {
        if (((fadeIn_pos <= 0) && (fadeOut_pos <= 0)) && (ch_pos < 0)) {
          alpha = 0;
        } else {
          alpha = 100 * (fadeOut_pos - ch_pos) / pimpl->fade_width;
        }
      }

      // 透過率を設定
      if (alpha != pimpl->surfaces_alpha[index]) {
        (*it)->setAlpha(alpha);
        pimpl->surfaces_alpha[index] = alpha;
        pimpl->is_changed |= true;
      }
    }
  }
}
