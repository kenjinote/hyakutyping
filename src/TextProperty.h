#ifndef TEXT_PROPERTY_H
#define TEXT_PROPERTY_H

/*!
  \file
  \brief Text ï`âÊÇÃèÓïÒä«óù

  \author Satofumi KAMIMURA

  $Id$
*/

#include "GuiColors.h"


namespace beego {
  class TtfResource;

  class TextProperty {
    TextProperty(void);
    //TextProperty(const TextProperty& rhs);
    //TextProperty& operator = (const TextProperty& rhs);

  public:
    TtfResource* font;
    const char* text;
    const Uint16* utext;
    size_t size;
    Uint32 fore_color;
    Uint32 back_color;
    bool transparent;

    TextProperty(TtfResource* font_, const char* text_, size_t size_,
                 Uint32 fore_color_ = beego::White,
                 Uint32 back_color_ = beego::Black,
                 bool transparent_ = false)
      : font(font_), text(text_), utext(NULL), size(size_),
        fore_color(fore_color_), back_color(back_color_),
        transparent(transparent_) {
    }

    TextProperty(TtfResource* font_, const Uint16* text_, size_t size_,
                 Uint32 fore_color_ = beego::White,
                 Uint32 back_color_ = beego::Black,
                 bool transparent_ = false)
      : font(font_), text(NULL), utext(text_), size(size_),
        fore_color(fore_color_), back_color(back_color_),
        transparent(transparent_) {
    }
  };
};

#endif /* !TEXT_PROPERTY_H */
