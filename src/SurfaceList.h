#ifndef SURFACE_LIST_H
#define SURFACE_LIST_H

/*!
  \file
  \brief サーフェスリスト

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Surface.h"
#include <SDL.h>
#include <vector>
#include <list>


namespace beego {
  /*!
    \brief 描画サーフェスの管理
  */
  class DrawSurface {
    static int serial_id;

  public:
    bool mustDraw;              //!< 描画する必要性の有無
    SDL_Rect position;          //!< 位置
    SDL_Rect area;              //!< オフセット、サイズ
    Surface surface;            //!< サーフェス
    bool position_changed;      //!< 配置が変更されたか
    bool is_changed;
    bool is_transparent;
    std::vector<SDL_Rect> redraw_rects; //!< 再描画領域
  };

  /*!
    \brief 描画サーフェスリスト
  */
  typedef std::list<DrawSurface*> SurfaceList;
};

#endif /* !SURFACE_LIST_H */
