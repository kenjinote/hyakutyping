#ifndef OPTION_MENU_COMPONENT_H
#define OPTION_MENU_COMPONENT_H

/*!
  \file
  \brief オプション用メニューコンポーネント

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include "Component.h"
#include "PlaceOption.h"
#include <boost/shared_ptr.hpp>
#include <memory>


namespace beego {
  class TextProperty;

  class OptionMenuComponent : public ComponentInterface {
    OptionMenuComponent(const OptionMenuComponent& rhs);
    OptionMenuComponent& operator = (const OptionMenuComponent& rhs);

    struct pImpl;
    std::auto_ptr<pImpl> pimpl;

  public:
    OptionMenuComponent(void);
    ~OptionMenuComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    void setIconComponent(Component icon,
                          int x_offset, int y_offset, int mode = None);
    void addItem(const TextProperty& title,
                 boost::shared_ptr<ComponentInterface> component,
                 int x_offset = 0, int y_offset = 0,
                 int place_mode = None);
    void setItemsOffset(int x_offset, int y_offset);
    void addItemOffset(int x_offset, int y_offset);

    // !!! 常にコンポーネントに入力を与えるためのメソッド
    // !!! マウス入力限定にできるか？
  };
};

#endif /* !OPTION_MENU_COMPONENT_H */
