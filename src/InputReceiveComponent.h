#ifndef INPUT_RECEIVE_COMPONENT_H
#define INPUT_RECEIVE_COMPONENT_H

/*!
  \file
  \brief 入力受け取り用のコンポーネント

  \author Satofumi KAMIMURA

  $Id$

  \todo GuiManager に含めるべきかを検討する
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  class InputReceiveComponent : public ComponentInterface {
    InputReceiveComponent(const InputReceiveComponent& rhs);
    InputReceiveComponent& operator = (const InputReceiveComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    InputReceiveComponent(void);
    ~InputReceiveComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    void clear(void);
    void updateInputEvent(InputEvent& event);
  };
};

#endif /* !INPUT_RECEIVE_COMPONENT_H */
