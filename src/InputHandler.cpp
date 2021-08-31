/*!
  \file
  \brief 入力処理

  \author Satofumi KAMIMURA

  $Id$

  \todo 排他制御を適切に行う
  \todo Alt-F4 などの操作を Windows 用入力セットとして管理できるようにする
  \todo emacs 入力セットの追加
  \todo 移動がないときにカーソルを隠す機能を追加
*/

#include "InputHandler.h"
#include "InputEvent.h"
#include "SdlInit.h"
#include "LockGuard.h"
#include <set>
#include <map>

using namespace std;
using namespace beego;


/*!
  \brief InputHandler の内部クラス
*/
struct InputHandler::pImpl : private SdlInit {
  // !!! この mutex の必要性は、再検討すべきかも
  SDL_mutex* mutex;
  bool quit_event;
  bool toggle_event_;
  typedef std::map<size_t,InputEvent> EventMap;
  EventMap events;
  size_t serial_id;

#if 0
  typedef struct {
    SDLKey sym;
    bool pressed;
  } key_t;
#endif

  typedef set<SDLKey> KeyReleased;
  typedef set<SDLKey> KeyPressed;
  KeyReleased key_released;
  KeyPressed key_pressed;

  pImpl(void) : mutex(SDL_CreateMutex()),
                quit_event(false), toggle_event_(false), serial_id(0) {
    SDL_EnableUNICODE(1);
  }

  ~pImpl(void) {
    SDL_DestroyMutex(mutex);
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    ++obj.serial_id;
    return &obj;
  }

  void setEvent(size_t id) {
    InputEvent event_buffer;
    events.insert(std::pair<size_t,InputEvent>(id, event_buffer));
  }

  void unsetEvent(size_t id) {
    events.erase(id);
  }

  void keyUpHandler(SDL_KeyboardEvent& key_event) {
    SDLKey sym = key_event.keysym.sym;
    SDLMod mod = key_event.keysym.mod;
    Uint16 unicode = key_event.keysym.unicode;
    key_released.insert(sym);

    for (EventMap::iterator it = events.begin(); it != events.end(); ++it) {
      it->second.key_released.push_back(sym);
    }

    for (EventMap::iterator it = events.begin(); it != events.end(); ++it) {
      it->second.key_pressed.
        push_back(InputEvent::InputKey(sym, mod, unicode, false));
    }
  }

  void keyDownHandler(SDL_KeyboardEvent& key_event) {

    SDLKey sym = key_event.keysym.sym;
    SDLMod mod = key_event.keysym.mod;
    Uint16 unicode = key_event.keysym.unicode;
    key_pressed.insert(sym);

    // 入力文字列の記録
    for (EventMap::iterator it = events.begin(); it != events.end(); ++it) {
      it->second.key_pressed.
        push_back(InputEvent::InputKey(sym, mod, unicode, true));
    }

    // フルスクリーン切り替え
    if ((mod & KMOD_ALT) && (sym == SDLK_RETURN)) {
      toggle_event_ = true;
    }

    // 終了イベントの検出
    if (mod & KMOD_CTRL) {
      if (sym == SDLK_q) {
        // Ctrl + q
        quit_event = true;
      } else if (sym == SDLK_F4) {
        // Alt + F4
        quit_event = true;
      }
    }
  }

  void mouseButtonHandler(SDL_MouseButtonEvent& btn_event) {
    if (btn_event.button == SDL_BUTTON_LEFT) {
      if (btn_event.type == SDL_MOUSEBUTTONUP) {
        for (EventMap::iterator it = events.begin();
             it != events.end(); ++it) {
          it->second.left_released = true;
          it->second.left_pressing = false;
        }
      } else if (btn_event.type == SDL_MOUSEBUTTONDOWN) {
        for (EventMap::iterator it = events.begin();
             it != events.end(); ++it) {
          it->second.left_clicked = true;
          it->second.left_pressing = true;
        }
      }
    } else if (btn_event.button == SDL_BUTTON_RIGHT) {

      // !!! この範囲は、上と同じなので、くくるべき
      if (btn_event.type == SDL_MOUSEBUTTONUP) {
        for (EventMap::iterator it = events.begin();
             it != events.end(); ++it) {
          it->second.right_released = true;
          it->second.right_pressing = false;
        }
      } else if (btn_event.type == SDL_MOUSEBUTTONDOWN) {
        for (EventMap::iterator it = events.begin();
             it != events.end(); ++it) {
          it->second.right_clicked = true;
          it->second.right_pressing = true;
        }
      }
    }

    int wheels_offset = 0;
    if (btn_event.type == SDL_MOUSEBUTTONDOWN) {
      if (btn_event.button == SDL_BUTTON_WHEELUP) {
        --wheels_offset;

      } else if (btn_event.button == SDL_BUTTON_WHEELDOWN) {
        ++wheels_offset;
      }
    }
    for (pImpl::EventMap::iterator it = events.begin();
         it != events.end(); ++it) {
      it->second.wheels += wheels_offset;
    }
  }

  void mouseMotionHandler(SDL_MouseMotionEvent& motion_event) {
    for (EventMap::iterator it = events.begin(); it != events.end(); ++it) {
      it->second.mouse_moved = true;
    }
  }

  void event_clear(size_t id) {
    InputEvent& it = events[id];
    it.mouse_moved = false;
    it.left_released = false;
    it.left_clicked = false;
    it.right_released = false;
    it.right_clicked = false;
    it.key_pressed.clear();
    it.key_released.clear();
    it.wheels = 0;
  }
};


InputHandler::InputHandler(void)
  // !!! 厳密には、getObject から serial_id を格納するまでを、ロックすべき
  : pimpl(pImpl::getObject()), own_id(pimpl->serial_id) {
  pimpl->setEvent(own_id);
}


InputHandler::~InputHandler(void) {
  pimpl->unsetEvent(own_id);
}


/*!
  \attention update() を呼び出す場合は state_clear() を定期的に呼び出すこと
*/
void InputHandler::state_clear(void) {
  LockGuard guard(pimpl->mutex);
  pimpl->key_released.clear();
  pimpl->key_pressed.clear();
}


void InputHandler::update_each(SDL_Event& event) {

  LockGuard guard(pimpl->mutex);

  switch (event.type) {
  case SDL_QUIT:
    pimpl->quit_event = true;
    break;

  case SDL_KEYUP:
    pimpl->keyUpHandler(event.key);
    break;

  case SDL_KEYDOWN:
    pimpl->keyDownHandler(event.key);
    break;

  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    pimpl->mouseButtonHandler(event.button);
    break;

  case SDL_MOUSEMOTION:
    pimpl->mouseMotionHandler(event.motion);
    break;
  }

  int mx = 0, my = 0;
  SDL_GetMouseState(&mx, &my);
  for (pImpl::EventMap::iterator it = pimpl->events.begin();
       it != pimpl->events.end(); ++it) {
    it->second.mx = mx;
    it->second.my = my;
  }
}


void InputHandler::update_all(void) {

  state_clear();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    update_each(event);
  }
}


bool InputHandler::haveQuitEvent(void) {
  return pimpl->quit_event;
}


bool InputHandler::haveToggleScreenEvent(void) {
  bool ret = pimpl->toggle_event_;
  pimpl->toggle_event_ = false;
  return ret;
}


bool InputHandler::isReleased(SDLKey key) {
  pImpl::KeyReleased::iterator p = pimpl->key_released.find(key);
  return (p == pimpl->key_released.end()) ? false : true;
}


bool InputHandler::isPressed(SDLKey key) {
  pImpl::KeyPressed::iterator p = pimpl->key_pressed.find(key);
  return (p == pimpl->key_pressed.end()) ? false : true;
}


void InputHandler::updateInputEvent(InputEvent& event) {
  LockGuard guard(pimpl->mutex);
  pImpl::EventMap::iterator it = pimpl->events.find(own_id);
  if (it == pimpl->events.end()) {
    return;
  }
  event = it->second;
  pimpl->event_clear(own_id);
}
