#ifndef GUI_COLORS_H
#define GUI_COLORS_H

/*!
  \file
  \brief 色の定義

  \author Satofumi KAMIMURA

  $Id$

  \todo エンディアンの差異を吸収させたい
  \todo GL 版からも呼ばれるので、getSdlColor() は別ファイルに移動させる
*/

#include <SDL.h>


namespace beego {

  /*!
    \brief 色の定義
  */
  enum {
    Black =  0x000000ff,         //!< 黒
    Blue =   0x0000ffff,         //!< 青
    Red =    0xff0000ff,         //!< 赤
    Purple = 0xff00ffff,         //!< 青
    Green =  0x00ff00ff,         //!< 緑
    Cyan =   0x00ffffff,         //!< 水色
    Yellow = 0xffff00ff,         //!< 黄色
    White =  0xffffffff,         //!< 白

    Gray0 =  0x000000ff,
    Gray1 =  0x101010ff,
    Gray2 =  0x202020ff,
    Gray3 =  0x303030ff,
    Gray4 =  0x404040ff,
    Gray5 =  0x505050ff,
    Gray6 =  0x606060ff,
    Gray7 =  0x707070ff,
    Gray8 =  0x808080ff,
    Gray9 =  0x909090ff,
    Gray10 = 0xa0a0a0ff,
    Gray11 = 0xb0b0b0ff,
    Gray12 = 0xc0c0c0ff,
    Gray13 = 0xd0d0d0ff,
    Gray14 = 0xe0e0e0ff,
    Gray15 = 0xf0f0f0ff,
  };

  extern Uint32 getSdlColor(const SDL_Surface* scr, Uint32 draw_color);
};

#endif /* !GUI_COLORS_H */
