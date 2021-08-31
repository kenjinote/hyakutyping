/*!
  \file
  \brief ヘッダファイルからのサーフェス作成

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CreateSurfaceFromArray.h"


SDL_Surface* beego::createSurface(const unsigned char data[],
                                  int width, int height) {

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  Uint32 rmask = 0xff000000;
  Uint32 gmask = 0x00ff0000;
  Uint32 bmask = 0x0000ff00;
  Uint32 amask = 0x000000ff;
#else
  Uint32 rmask = 0x000000ff;
  Uint32 gmask = 0x0000ff00;
  Uint32 bmask = 0x00ff0000;
  Uint32 amask = 0xff000000;
#endif

  return SDL_CreateRGBSurfaceFrom(const_cast<unsigned char*>(data),
                                  width, height, 32, width * 4,
                                  rmask, gmask, bmask, amask);
}
