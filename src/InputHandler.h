#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

/*!
  \file
  \brief 入力処理

  \author Satofumi KAMIMURA

  $Id$

  \todo vector<InputKey> を InputKeys に置き換える
  \todo vector<SDLKey> を SdlKeys に置き換える
*/

#include <SDL.h>
//#include <boost/shared_ptr.hpp>


namespace beego {
  class InputEvent;

  /*!
    \brief 入力情報の管理
  */
  class InputHandler {
    InputHandler(const InputHandler& rhs);
    InputHandler& operator = (const InputHandler& rhs);

    struct pImpl;
    pImpl* pimpl;
    //const boost::shared_ptr<pImpl> pimpl;
    size_t own_id;

  public:
    InputHandler(void);
    ~InputHandler(void);

    void state_clear(void);
    void update_each(SDL_Event& event);
    void update_all(void);
    void getMousePosition(int* x, int* y);
    bool haveQuitEvent(void);
    bool haveToggleScreenEvent(void);
    bool isReleased(SDLKey key);
    bool isPressed(SDLKey key);
    void updateInputEvent(InputEvent& event);
  };
};

#endif /* !INPUT_HANDLER_H */
