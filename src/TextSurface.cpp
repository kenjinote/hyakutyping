/*!
  \file
  \brief �����T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TextSurface.h"
#include "TextProperty.h"
#include "Surface.h"
#include "TtfResource.h"
#include "SdlUtils.h"
#include "SdlSurface.h"

using namespace beego;


struct TextSurface::pImpl {
  boost::shared_ptr<SdlSurface> surface;

  void initialize(TtfResource* font, const Uint16* text_data, size_t size,
                  Uint32 fore, Uint32 back, bool transparent) {

    // �T�[�t�F�X�̍쐬
    SDL_Color fore_color;
    set_SdlColor(&fore_color, fore);
    SDL_Color back_color;
    set_SdlColor(&back_color, back);
    boost::shared_ptr<SdlSurface>
      new_surface(new SdlSurface
                  (TTF_RenderUNICODE_Shaded(font->getResource(size),
                                            &text_data[0],
                                            fore_color, back_color),
                   transparent));
    std::swap(surface, new_surface);
  }

  pImpl(const TextProperty& property) {

    if (property.text) {
      // const char* �����񂪓o�^����Ă���ꍇ�́A
      // Uint16 �����������ăT�[�t�F�X�𐶐�����
      std::vector<Uint16> text_data;
      for (int i = 0; property.text[i] != 0x0; ++i) {
        text_data.push_back(property.text[i]);
      }
      text_data.push_back(0x0);

      initialize(property.font, &text_data[0], property.size,
                 property.fore_color, property.back_color,
                 property.transparent);
    } else {
      initialize(property.font, property.utext, property.size,
                 property.fore_color, property.back_color,
                 property.transparent);
    }
  }

  ~pImpl(void) {
    //surface.clear();
    // !!! �K�v�Ȃ񂾂����H ���Aclear() ���ĉ��H
  }
};


TextSurface::TextSurface(const TextProperty& text_property)
  : pimpl(new pImpl(text_property)) {
}


TextSurface::~TextSurface(void) {
}


void TextSurface::draw(std::vector<SDL_Rect>& update_rects,
                       const SDL_Rect* pos, const SDL_Rect* area) {
  pimpl->surface->draw(update_rects, pos, area);
}


size_t TextSurface::getWidth(void) {
  return pimpl->surface->getWidth();
}


size_t TextSurface::getHeight(void) {
  return pimpl->surface->getHeight();
}


void TextSurface::forceSetChanged(void) {
  pimpl->surface->forceSetChanged();
}


bool TextSurface::isChanged(size_t ticks) {
  return pimpl->surface->isChanged(ticks);
}


bool TextSurface::isTransparent(void) {
  return pimpl->surface->isTransparent();
}


void TextSurface::setAlpha(size_t percent) {

  // ���l���X�V
  pimpl->surface->setAlpha(percent);
}
