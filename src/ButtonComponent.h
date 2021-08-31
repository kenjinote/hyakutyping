#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

/*!
  \file
  \brief ボタンコンポーネント

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>

namespace beego {
  class TtfResource;
  class TextProperty;

  class ButtonComponent : public ComponentInterface {
    ButtonComponent(const ButtonComponent& rhs);
    ButtonComponent& operator = (const ButtonComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      NoSelect = -1,
    };
    ButtonComponent(void);
    ~ButtonComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! 大きさの違うサーフェスを登録したい場合には、どうするのかな？
    // !!! - 大きさは同じにする
    // !!! - 最大サイズを指定しておき、サーフェスと位置を指定できるようにする
    // !!! - 異なるサーフェスでも、問題ないと見なす
    // !!!   - サーフェスの基準位置を指定する
    // !!!     - 中心基準、左揃え、右揃え、を上下、左右毎に

    // !!! 描画サーフェスサイズと、それの配置オフセットを指定可能にする

    // 基準サーフェスを登録
    void registerNormalSurface(Surface surface,
                               int x_offset =0, int y_offset = 0);

    // 押下時サーフェスを登録
    void registerPressedSurface(Surface surface,
                                int x_offset =0, int y_offset = 0);

    // フォーカス時サーフェスを登録
    void registerFocusedSurface(Surface surface,
                                int x_offset =0, int y_offset = 0);

    Surface getNormalSurface(void);

    void showNormalSurface(void);
    void showPressedSurface(void);
    void showFocusedSurface(void);

    // !!! 表示サーフェスをユーザが指定するか、キーで指定するか、を選択させる

    bool isChanged(void);
    bool isDecided(void);
    void setDecided(void);
    void releaseDecided(void);

    static void createButton(boost::shared_ptr<ButtonComponent>& button,
                             const TextProperty& normal,
                             const TextProperty& selected,
                             const TextProperty& pressed);
  };
};

#endif /* !BUTTON_COMPONENT_H */
