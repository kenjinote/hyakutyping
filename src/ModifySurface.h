#ifndef MODIFY_SURFACE_H
#define MODIFY_SURFACE_H

/*!
  \file
  \brief サーフェスフォーマットの変更

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>


namespace beego {
  extern SDL_Surface* transparent(SDL_Surface* scr, SDL_Surface* surface);
  extern size_t setAlpha(size_t percent, size_t now_percent,
                         SDL_Surface* surface);
};

#endif /* !MODIFY_SURFACE_H */
