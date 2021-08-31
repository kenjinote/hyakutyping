#ifndef SELECT_COMPONENT_H
#define SELECT_COMPONENT_H

/*!
  \file
  \brief 水平セレクタ

  \author Satofumi KAMIMURA

  $Id$

  \todo 名前長すぎ、改名する
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  class TextProperty;

  class SelectComponent : public ComponentInterface {
    SelectComponent(void);
    SelectComponent(const SelectComponent& rhs);
    SelectComponent& operator = (const SelectComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      NotSelected = -1,
    };
    typedef enum {
      BackColor,
      UnderBar,
    } SelectMode;
    explicit SelectComponent(const TextProperty& text_property,
                             Uint32 active_color,
                             SelectMode mode = BackColor);

    ~SelectComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    void enableInput(void);
    void disableInput(void);

    // !!! 項目を追加
    void addItem(const char* text);
    void addItem(const Uint16* utext);
    void setItemsOffset(int x_offset);
    int getSelected(void);
    void setSelected(int index);

    int getDecided(void);
    //void setDecided(int index);
    void releaseDecided(void);

    // !!! 項目の間隔を設定
    // !!! アンダーバー or 背景色を指定
  };
};

#endif /* !SELECT_COMPONENT_H */
