#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

/*!
  \file
  \brief ì¸óÕÉCÉxÉìÉgÇÃä«óù

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


namespace beego {
  class InputEvent {
  public:
    int mx;
    int my;
    bool mouse_moved;
    int wheels;

    bool left_clicked;
    bool left_released;
    bool left_pressing;

    bool right_clicked;
    bool right_released;
    bool right_pressing;

    class InputKey {
    public:
      SDLKey sym_;
      SDLMod mod_;
      Uint16 unicode_;
      bool pressed_;

      InputKey(SDLKey sym, SDLMod mod, Uint16 unicode, bool pressed)
        : sym_(sym), mod_(mod), unicode_(unicode),
          pressed_(pressed) {
      }
    };
    typedef std::vector<InputKey> InputKeys;
    typedef std::vector<SDLKey> SdlKeys;
    InputKeys key_pressed;
    SdlKeys key_released;

    InputEvent(void)
      : mx(0), my(0), mouse_moved(false), wheels(0),
        left_clicked(false), left_released(false), left_pressing(false),
        right_clicked(false), right_released(false), right_pressing(false) {
    }

    static bool isPressed(const InputEvent& event, SDLKey key);
    static bool isReleased(const InputEvent& event, SDLKey key);
  };
};

#endif /* !INPUT_EVENT_H */
