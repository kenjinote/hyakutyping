#ifndef SURFACE_INTERFACE_H
#define SURFACE_INTERFACE_H

/*!
  \file
  \brief サーフェスインターフェース

  \author Satofumi KAMIMURA

  $Id$

  サーフェスとは、描画資源を管理するもの

  \todo ticks で描画内容が変わる場合は、全体の再描画が必要かも。検討する
  \todo 削除用に、描画したときの ticks を恒久的に記録する仕組みが必要か検討する
*/

#include <SDL.h>
#include <vector>


namespace beego {
  /*!
    \brief 描画サーフェスのインターフェース定義
  */
  class SurfaceInterface {
  public:
    virtual ~SurfaceInterface() {}

    /*!
      \brief サーフェスの実描画

      \param pos [i] 描画位置
      \param area [i] 描画オフセット、および描画サイズ

      描画位置を始点として、描画オフセットから描画サイズだけ描画する
    */
    virtual void draw(std::vector<SDL_Rect>& update_rects,
                      const SDL_Rect* pos, const SDL_Rect* area) = 0;
    virtual size_t getWidth(void) = 0;
    virtual size_t getHeight(void) = 0;
    virtual void forceSetChanged(void) = 0;
    virtual bool isChanged(size_t ticks) = 0;
    virtual bool isTransparent(void) = 0;
  };
};

#endif /* !SURFACE_INTERFACE_H */
