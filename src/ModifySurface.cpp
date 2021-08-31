/*!
  \file
  \brief サーフェスフォーマットの変更

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ModifySurface.h"


SDL_Surface* beego::transparent(SDL_Surface* scr, SDL_Surface* surface) {
  if ((surface == NULL) || (scr == NULL)) {
    return NULL;
  }

  Uint32 trans_color = 0;
  if (surface->format->BitsPerPixel == 8) {
    // ピクセルフォーマットが 8 bit の場合
    Uint8 index = *(Uint8 *)surface->pixels;
    SDL_Color& color = surface->format->palette->colors[index];
    trans_color = SDL_MapRGB(surface->format, color.r, color.g, color.b);
  } else {
    trans_color = *(Uint32*)surface->pixels;
  }

  SDL_SetColorKey(surface,
                  SDL_SRCCOLORKEY | SDL_RLEACCEL, trans_color);
  SDL_Surface* transparent_surface = SDL_DisplayFormat(surface);
  SDL_FreeSurface(surface);
  surface = transparent_surface;

  return surface;
}


size_t beego::setAlpha(size_t percent, size_t now_percent,
                       SDL_Surface* surface) {

  // !!! いや、この独立してない関数は、どうなのよ？ と思った
  // !!! 気を見て作り直すべき。既に、gl_gui からは使ってない
  // !!! SdlSurface.cpp に戻してもよいかも

  percent = (percent > 100) ? 100 : percent;
  if ((percent != now_percent) && surface) {

    SDL_SetAlpha(surface, SDL_SRCALPHA,
                 static_cast<Uint8>(percent * SDL_ALPHA_OPAQUE / 100));
    now_percent = percent;
  }

  return now_percent;
}
