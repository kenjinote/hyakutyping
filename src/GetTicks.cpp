/*!
  \file
  \brief 時間間隔の取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlInit.h"
#include <SDL.h>
#include "GetTicks.h"


/*!
  \brief SdlInit の初期化クラス
*/
class SdlInitClass : private beego::SdlInit {
};


size_t beego::GetTicks(void) {
  static SdlInitClass sdl_init;
  return SDL_GetTicks();
}
