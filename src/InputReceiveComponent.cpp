/*!
  \file
  \brief 入力受け取り用のコンポーネント

  \author Satofumi KAMIMURA

  $Id$

  \todo GuiManager に含めるべきかを検討する
*/

#include "InputReceiveComponent.h"
#include "InputHandler.h"
#include "InputEvent.h"
#include "SdlUtils.h"

using namespace beego;


struct InputReceiveComponent::pImpl {
  InputEvent input_event;
  InputHandler input;

  void updateEvent(const InputEvent& event) {
    input_event.left_released = event.left_released;
    input_event.mx = event.mx;
    input_event.my = event.my;

    // !!! いずれ、mearge? に置き換える
    for (InputEvent::InputKeys::const_iterator it = event.key_pressed.begin();
         it != event.key_pressed.end(); ++it) {
      input_event.key_pressed.push_back(*it);
    }

    // !!! いずれ、mearge? に置き換える
    for (InputEvent::SdlKeys::const_iterator it = event.key_released.begin();
         it != event.key_released.end(); ++it) {
      input_event.key_released.push_back(*it);
    }
  }

  void clearEvent(void) {
    input_event.left_released = false;
    input_event.key_pressed.clear();
    input_event.key_released.clear();
  }
};


InputReceiveComponent::InputReceiveComponent(void) : pimpl(new pImpl) {
}


InputReceiveComponent::~InputReceiveComponent(void) {
}


void InputReceiveComponent::addSurfaceList(SurfaceList& surfaces,
                                           const SDL_Rect* area,
                                           size_t ticks) {
  // 実装しない
}


void InputReceiveComponent::setPosition(const SDL_Rect* position) {
  // 実装しない
}


void InputReceiveComponent::getPosition(SDL_Rect* position) {
  set_SdlRect(position, 0, 0, 0, 0);
}


size_t InputReceiveComponent::getWidth(void) {
  return 0;
}


size_t InputReceiveComponent::getHeight(void) {
  return 0;
}


void InputReceiveComponent::applyInput(const InputEvent& event,
                                       std::vector<SDL_Rect>& applied_rects) {
  pimpl->updateEvent(event);
}


void InputReceiveComponent::registerLayer(Layer* layer) {
  // 何もしないp
}


void InputReceiveComponent::clear(void) {
  pimpl->clearEvent();
}


void InputReceiveComponent::updateInputEvent(InputEvent& event) {
  event = pimpl->input_event;
  clear();
}
