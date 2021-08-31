/*!
  \file
  \brief ŠÔŠÔŠu‚Ìæ“¾

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlInit.h"
#include <SDL.h>
#include "GetTicks.h"


/*!
  \brief SdlInit ‚Ì‰Šú‰»ƒNƒ‰ƒX
*/
class SdlInitClass : private beego::SdlInit {
};


size_t beego::GetTicks(void) {
  static SdlInitClass sdl_init;
  return SDL_GetTicks();
}
