#ifndef SDL_UTILS_H
#define SDL_UTILS_H

/*!
  \file
  \brief SDL �ϐ��̑���⏕

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


namespace beego {
  /*!
    \brief SDL_Rect �ւ̑���֐�

    \param rect [o] �����
    \param x [i] ����l
    \param y [i] ����l
    \param w [i] ����l
    \param h [i] ����l
  */
  extern void set_SdlRect(SDL_Rect* rect, int x, int y, int w = 0, int h = 0);

  extern void add_SdlRect(std::vector<SDL_Rect>& list,
                          int x, int y, int w = 0, int h = 0);

  /*!
    \brief SDL_Color �ւ̑���֐�

    \param color [o] �����
    \param uint32_color [i] Uint32 �\���̐F�B0xrrbbggaa �`��
  */
  extern void set_SdlColor(SDL_Color* color, Uint32 uint32_color);
};

#endif /* !SDL_UTILS_H */
