/*!
  \file
  \brief SDL CDROM の初期化

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlCdromInit.h"
#include "SdlInit.h"
#include "LogManager.h"
#include <SDL.h>
#include <stdlib.h>
#include <string>

using namespace beego;


struct SdlCdromInit::pImpl : private SdlInit {
  static bool initialized;
};

bool SdlCdromInit::pImpl::initialized = false;


SdlCdromInit::SdlCdromInit(void) : pimpl(new pImpl) {
  if (pimpl->initialized == false) {
    if (SDL_InitSubSystem(SDL_INIT_CDROM) < 0) {
      // ログ記録
      LogManager* log = LogManager::getObject();
      std::string message = "SDL_InitSubSystem(SDL_INIT_CDROM): " +
        std::string(SDL_GetError());
      log->write(LogManager::Error, message.c_str());
    }
    atexit(SDL_Quit);
    pimpl->initialized = true;
  }
}


SdlCdromInit::~SdlCdromInit(void) {
}


bool SdlCdromInit::isInitialized(void) {
  return pimpl->initialized;
}
