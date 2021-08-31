/*!
  \file
  \brief �F�̒�`

  \author Satofumi KAMIMURA

  $Id$

  \todo �l�[���X�y�[�X�𓱓�����
*/

#include "GuiColors.h"


Uint32 beego::getSdlColor(const SDL_Surface* scr, Uint32 draw_color) {
  return SDL_MapRGBA(scr->format,
                     static_cast<Uint8>((draw_color >> 24) & 0xff),
                     static_cast<Uint8>((draw_color >> 16) & 0xff),
                     static_cast<Uint8>((draw_color >>  8) & 0xff),
                     static_cast<Uint8>(draw_color & 0xff));
}
