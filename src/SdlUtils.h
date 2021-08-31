#ifndef SDL_UTILS_H
#define SDL_UTILS_H

/*!
  \file
  \brief SDL 変数の操作補助

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


namespace beego {
  /*!
    \brief SDL_Rect への代入関数

    \param rect [o] 代入先
    \param x [i] 代入値
    \param y [i] 代入値
    \param w [i] 代入値
    \param h [i] 代入値
  */
  extern void set_SdlRect(SDL_Rect* rect, int x, int y, int w = 0, int h = 0);

  extern void add_SdlRect(std::vector<SDL_Rect>& list,
                          int x, int y, int w = 0, int h = 0);

  /*!
    \brief SDL_Color への代入関数

    \param color [o] 代入先
    \param uint32_color [i] Uint32 表現の色。0xrrbbggaa 形式
  */
  extern void set_SdlColor(SDL_Color* color, Uint32 uint32_color);
};

#endif /* !SDL_UTILS_H */
