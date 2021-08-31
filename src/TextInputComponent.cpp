/*!
  \file
  \brief ���͕����̕\���R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$

  \todo �I�[�g���s�[�g����������
  \todo �̈悩��͂ݏo���������T�[�t�F�X�͕`�悵�Ȃ�
  \todo ���ߎ��́A�w�i�F�����߂ɂ���
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
    // ustrcat ���I�[�� 0x0 ��ǉ�����̂ŁA��菜��
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

      // �쐬�����T�[�t�F�X���\���̈���z������A����
      if ((new_surface->getWidth() + x_offset) > width) {
        inputed_text.pop_back();
        return;
      }
      std::swap(text_surface, new_surface);
    }

    // �ʒu���ς�������Ƃɂ��āA�ĕ`�悷��
    // !!! �ł���΁A�ʂ̘g�g�݂łȂ�Ƃ��������Ȃ�...
    // !!! ������\����p�̃T�[�t�F�X�����΁A����őΏ��ł��邯�ǂȂ�...
    // !!! \todo ���̂������
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

  // !!! ��̎��ɂ́A����̃T�[�t�F�X��o�^���Ă��܂��̂��A�肩��
  // !!! NullSurface ��o�^���邱�Ƃɂ���
  // �����T�[�t�F�X�̔z�u

  // !!! �{���́A���̂�����ŃT�[�t�F�X�����ׂ��A���H
  // !!! �\���������\���ɑ�����΁A�ǂ����ł��悢�b�����ȁI

  // ������T�[�t�F�X�̓o�^
  SDL_Rect text_position = pimpl->position;
  text_position.x += pimpl->x_offset;
  text_position.y += pimpl->y_offset;
  text_position.w = pimpl->text_surface->getWidth();
  text_position.h = pimpl->text_surface->getHeight();

  updateDrawSurface(pimpl->text_draw_surface, pimpl->text_surface, area,
                    ticks, text_position, pimpl->position_changed);
  surfaces.push_back(&pimpl->text_draw_surface);

  // �w�i�T�[�t�F�X�̓o�^
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
    // ���͊m���́A�������͂��󂯕t���Ȃ�
    return;
  }

  typedef std::vector<InputEvent::InputKey> Input;

  // !!! ���͂�ϊ����Ă̕\��
  bool text_changed = false;
  for (Input::const_iterator it = event.key_pressed.begin();
       it != event.key_pressed.end(); ++it) {

    // ����L�[�͓��ʈ����Ƃ���
    // !!! �{���́A���s��̃L�[���͂��K�؂ɏ������ׂ�
    // !!! ... �K�؂��ĂȂ񂾁H ����ς�A�̂Ă邵�������񂶂�Ȃ��́H

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
          // SHIFT + 0 �������ꂽ�ꍇ�A���ȓ��͂̃T�|�[�g�̂��߂�
          // ~ �����ɕϊ����ēo�^����
          text_changed |= pimpl->converter->addChar('~');
          continue;

      } else if ((it->unicode_ & 0xff00) || (! isprint(it->unicode_))) {
        // �\���ł��Ȃ��L�[���͂́A�����Ƃ��Ĉ���Ȃ�
        continue;
      }
      // !!! �J�[�\���ړ�

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
  // �ĕ`��𑣂�
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

  // �ʒu���ς�������Ƃɂ��āA�ĕ`�悷��
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
