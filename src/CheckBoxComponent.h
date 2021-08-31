#ifndef CHECK_BOX_COMPONENT_H
#define CHECK_BOX_COMPONENT_H

/*!
  \file
  \brief チェックボックス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  class CheckBoxComponent : public ComponentInterface {
    CheckBoxComponent(const CheckBoxComponent& rhs);
    CheckBoxComponent& operator = (const CheckBoxComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    CheckBoxComponent(void);
    ~CheckBoxComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! ボックスの中心位置を取得できるメソッド
    // !!! 色の指定メソッド
    // !!! チェック画像
    void setSurface(Surface surface, int x_offset, int y_offset);
    bool isChecked(void);
    bool isCheckChanged(void);
    void setCheck(bool on);
    void toggleCheck(void);
  };
};

#endif /* !CHECK_BOX_COMPONENT_H */
