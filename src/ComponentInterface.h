#ifndef COMPONENT_INTERFACE_H
#define COMPONENT_INTERFACE_H

/*!
  \file
  \brief コンポーネントインターフェース

  \author Satofumi KAMIMURA

  $Id$

  コンポーネントとは、機能の実装を管理するもの (説明不足だな...)
*/

#include "SurfaceList.h"

namespace beego {
  class InputEvent;
  class Layer;

  /*!
    \brief コンポーネントインターフェース
  */
  class ComponentInterface {
  public:
    virtual ~ComponentInterface() {}
    virtual void addSurfaceList(SurfaceList& surfaces,
                                const SDL_Rect* area, size_t ticks) = 0;
    virtual void setPosition(const SDL_Rect* position) = 0;

    // !!! SDL_Rect getPosition(void) に変更するかな？
    virtual void getPosition(SDL_Rect* position) = 0;

    virtual size_t getWidth(void) = 0;
    virtual size_t getHeight(void) = 0;
    virtual void applyInput(const InputEvent& event,
                            std::vector<SDL_Rect>& applied_rects) = 0;

    // !!! 「配置」の意味を含むメソッド名に置き換える
    // !!! という改名は保留
    virtual void registerLayer(Layer* layer) = 0;
  };
};

#endif /* !COMPONENT_INTERFACE_H */
