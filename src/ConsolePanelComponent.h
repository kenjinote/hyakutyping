#ifndef CONSOLE_PANEL_COMPONENT_H
#define CONSOLE_PANEL_COMPONENT_H

/*!
  \file
  \brief コンソール・コンポーネント

  \author Satofumi KAMIMURA

  $Id$

  \todo スクロールアウトした場合に、スクロールバーを付加できるようにする
*/

#include "ComponentInterface.h"
#include "GuiColors.h"
#include <memory>


namespace beego {
  class TtfResource;
  class TextProperty;

  class ConsolePanelComponent : public ComponentInterface {
    ConsolePanelComponent(void);
    ConsolePanelComponent(const ConsolePanelComponent& rhs);
    ConsolePanelComponent& operator = (const ConsolePanelComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    typedef enum {
      Down = +1,
      Up = -1,
    } DrawDirection;

    typedef enum {
      FadeOut,
      ScrollOut,
    } EraseType;

    enum {
      DefaultFadePeriod = 2000,
    };

    explicit ConsolePanelComponent(size_t width, size_t height,
                                   const TextProperty& text_property,
                                   EraseType erase_type = ScrollOut,
                                   DrawDirection direction = Down);
    ~ConsolePanelComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    int printf(const char *format, ...);
    void clear(void);
    void setColor(Uint32 color, Uint32 back_color = Black);
    void setFadeOutMsec(size_t begin, size_t period = DefaultFadePeriod);
  };
};

#endif /* !DRAW_PANEL_COMPONENT_H */
