/*!
  \file
  \brief 入力文字の表示コンポーネント

  \author Satofumi KAMIMURA

  $Id$

  \todo オートリピートを実現する
  \todo 領域からはみ出した文字サーフェスは描画しない
  \todo 透過時は、背景色も透過にする
*/

#include "TextInputComponent.h"
#include "InputEvent.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "ColorSurface.h"
#include "NullSurface.h"
#include "Component.h"
#include "SdlUtils.h"
#include "PlainTextConverter.h"
#include "UtfString.h"

using namespace beego;


struct TextInputComponent::pImpl {
  SDL_Rect position;
  size_t width;
  size_t height;
  TtfResource* ttf;
  size_t font_size;
  Uint32 fore_color;
  Uint32 back_color;
  bool is_transparent;
  std::vector<Uint16> inputed_text;
  Surface text_surface;
  Surface back_surface;
  bool is_decided;
  bool position_changed;
  PlainTextConverter plain_converter;
  TextConvertInterface* converter;
  size_t x_offset;
  size_t y_offset;
  bool is_changed;
  DrawSurface text_draw_surface;
  DrawSurface back_draw_surface;
  std::vector<Uint16> default_text;
  bool is_erased;

  pImpl(size_t w, size_t h,
        const TextProperty& property, int xOffset, int yOffset)
    : width(w), height(h), ttf(property.font), font_size(property.size),
      fore_color(property.fore_color), back_color(property.back_color),
      is_transparent(property.transparent),
      back_surface(new ColorSurface(width, height, back_color)),
      is_decided(false), position_changed(false), converter(&plain_converter),
      x_offset(xOffset), y_offset(yOffset), is_changed(true),
      is_erased(false) {

    set_SdlRect(&position, 0, 0, width, height);
    updateSurface();

    if (property.text) {
      ustrcat(default_text, property.text);

    } else if (property.utext) {
      ustrcat(default_text, property.utext);
    }
    // ustrcat が終端に 0x0 を追加するので、取り除く
    default_text.pop_back();
  }

  void updateSurface(void) {
    converter->getConvertBuffer(inputed_text);

    if (inputed_text.empty()) {
      Surface null_surface(new NullSurface);
      std::swap(text_surface, null_surface);

    } else {
      inputed_text.push_back('\0');
      TextProperty text_property(ttf, &inputed_text[0], font_size,
                                 fore_color, back_color, is_transparent);
      Surface new_surface(new TextSurface(text_property));
      inputed_text.pop_back();

      // 作成したサーフェスが表示領域を越えたら、無効
      if ((new_surface->getWidth() + x_offset) > width) {
        inputed_text.pop_back();
        return;
      }
      std::swap(text_surface, new_surface);
    }

    // 位置が変わったことにして、再描画する
    // !!! できれば、別の枠組みでなんとかしたいなぁ...
    // !!! 文字列表示専用のサーフェスを作れば、それで対処できるけどなぁ...
    // !!! \todo そのうち作る
    position_changed = true;
    is_changed = true;
  }
};


TextInputComponent::TextInputComponent(size_t width, size_t height,
                                       const TextProperty& property,
                                       int x_offset, int y_offset)
  : pimpl(new pImpl(width, height, property, x_offset, y_offset)) {
  setBuffer(pimpl->default_text);
}


TextInputComponent::~TextInputComponent(void) {
}


void TextInputComponent::addSurfaceList(SurfaceList& surfaces,
                                        const SDL_Rect* area, size_t ticks) {

  // !!! 空の時には、代わりのサーフェスを登録してしまうのも、手かも
  // !!! NullSurface を登録することにした
  // 文字サーフェスの配置

  // !!! 本来は、このあたりでサーフェスを作るべき、か？
  // !!! 表示周期が十分に早ければ、どっちでもよい話だがな！

  // 文字列サーフェスの登録
  SDL_Rect text_position = pimpl->position;
  text_position.x += pimpl->x_offset;
  text_position.y += pimpl->y_offset;
  text_position.w = pimpl->text_surface->getWidth();
  text_position.h = pimpl->text_surface->getHeight();

  updateDrawSurface(pimpl->text_draw_surface, pimpl->text_surface, area,
                    ticks, text_position, pimpl->position_changed);
  surfaces.push_back(&pimpl->text_draw_surface);

  // 背景サーフェスの登録
  updateDrawSurface(pimpl->back_draw_surface, pimpl->back_surface, area,
                    ticks, pimpl->position, pimpl->position_changed);
  surfaces.push_back(&pimpl->back_draw_surface);

  pimpl->position_changed = false;
}


void TextInputComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position, position->x, position->y,
              pimpl->position.w, pimpl->position.h);
  pimpl->position_changed = true;
}


void TextInputComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position;
}


size_t TextInputComponent::getWidth(void) {
  return pimpl->position.w;
}


size_t TextInputComponent::getHeight(void) {
  return pimpl->position.h;
}


void TextInputComponent::applyInput(const InputEvent& event,
                                    std::vector<SDL_Rect>& applied_rects) {
  if (pimpl->is_decided) {
    // 入力確定後は、文字入力を受け付けない
    return;
  }

  typedef std::vector<InputEvent::InputKey> Input;

  // !!! 入力を変換しての表示
  bool text_changed = false;
  for (Input::const_iterator it = event.key_pressed.begin();
       it != event.key_pressed.end(); ++it) {

    // 特殊キーは特別扱いとする
    // !!! 本来は、改行後のキー入力も適切に処理すべき
    // !!! ... 適切ってなんだ？ やっぱり、捨てるしか無いんじゃないの？

    if (it->pressed_) {
      if (it->sym_ == SDLK_RETURN) {
        pimpl->is_decided = true;
        pimpl->is_changed = true;
        return;

      } else if (it->sym_ == SDLK_BACKSPACE) {
        text_changed |= pimpl->converter->deleteBack();
        pimpl->is_erased = true;
        continue;

      } else if (it->sym_ == SDLK_DELETE) {
        text_changed |= pimpl->converter->deleteCurrent();
        pimpl->is_erased = true;
        continue;

      } else if ((it->mod_ & KMOD_SHIFT) && (it->sym_ == SDLK_0)) {
          // SHIFT + 0 が押された場合、かな入力のサポートのために
          // ~ 文字に変換して登録する
          text_changed |= pimpl->converter->addChar('~');
          continue;

      } else if ((it->unicode_ & 0xff00) || (! isprint(it->unicode_))) {
        // 表示できないキー入力は、文字として扱わない
        continue;
      }
      // !!! カーソル移動

      text_changed |= pimpl->converter->addChar(it->unicode_);

    } else {
      if (it->sym_ == SDLK_ESCAPE) {
        text_changed |= pimpl->converter->escapePressed();
        pimpl->is_erased = true;
        continue;
      }
    }
  }

  if (text_changed) {
    pimpl->updateSurface();
  }
}


void TextInputComponent::registerLayer(Layer* layer) {
  // 再描画を促す
  //pimpl->text_surface->forceSetChanged();
  //pimpl->back_surface->forceSetChanged();
}


bool TextInputComponent::isDecided(void) {
  return pimpl->is_decided;
}


void TextInputComponent::releaseDecided(void) {
  pimpl->is_decided = false;
}


void TextInputComponent::clear(void) {

  pimpl->converter->clear();
  pimpl->updateSurface();

  // 位置が変わったことにして、再描画する
  pimpl->position_changed = true;
}


size_t TextInputComponent::getBuffer(std::vector<Uint16>& buffer) {

  buffer = pimpl->inputed_text;
  size_t size = buffer.size();
  buffer.push_back('\0');

  return size;
}


void TextInputComponent::setBuffer(std::vector<Uint16>& data) {

  pimpl->converter->setConvertBuffer(data);
  pimpl->updateSurface();
}


void TextInputComponent::swapTextConverter(TextConvertInterface* converter) {
  pimpl->converter = converter;
}


bool TextInputComponent::empty(void) {
  return pimpl->inputed_text.empty();
}


bool TextInputComponent::isChanged(void) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool TextInputComponent::isErased(void) {
  bool ret = pimpl->is_erased;
  pimpl->is_erased = false;
  return ret;
}
