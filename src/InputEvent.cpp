/*!
  \file
  \brief 入力イベントの管理

  \author Satofumi KAMIMURA

  $Id$

  \todo isPressed() を改名する。今は、解放情報も返している
*/

#include "InputEvent.h"

using namespace beego;


bool InputEvent::isPressed(const InputEvent& event, SDLKey key) {

  for (InputEvent::InputKeys::const_iterator it = event.key_pressed.begin();
       it != event.key_pressed.end(); ++it) {
    if (it->pressed_ && (it->sym_ == key)) {
      return true;
    }
  }
  return false;
}


bool InputEvent::isReleased(const InputEvent& event, SDLKey key) {

  for (InputEvent::SdlKeys::const_iterator it = event.key_released.begin();
       it != event.key_released.end(); ++it) {
    if (*it == key) {
      return true;
    }
  }
  return false;
}

