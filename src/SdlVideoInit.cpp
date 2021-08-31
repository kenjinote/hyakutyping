/*!
  \file
  \brief SDL_VIDEO の初期化クラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlVideoInit.h"
#include "SdlInit.h"
#include "LogManager.h"
#ifdef MSC
#include "SDL_win.h"
#else
#include <SDL.h>
#endif
#include <stdlib.h>
#include <string>

using namespace beego;


/*!
  \brief SdlVideoInit の内部クラス
*/
struct SdlVideoInit::pImpl : private SdlInit {
  static bool initialized;
};

bool SdlVideoInit::pImpl::initialized = false;


SdlVideoInit::SdlVideoInit(void) : pimpl(new pImpl) {

  // !!! 問題が発生するようであれば、double lock にする
  // !!! むしろ、シングルトンにするべきじゃねぇ？

  if (pimpl->initialized == false) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
      // ログ記録
      LogManager* log = LogManager::getObject();
      std::string message = "SDL_VideoInit(): " + std::string(SDL_GetError());
      log->write(LogManager::Error, message.c_str());
      throw;
    }
    pimpl->initialized = true;
  }
}


SdlVideoInit::~SdlVideoInit(void) {
}
