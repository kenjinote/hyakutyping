/*!
  \file
  \brief SDL ‰¹Šy‘Œ¹‚Ì‰Šú‰»

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlAudioInit.h"
#include "SdlInit.h"
#include "LogManager.h"
#include <SDL.h>
#include <stdlib.h>
#include <string>

using namespace beego;


struct SdlAudioInit::pImpl : private SdlInit {
  static bool initialized;
};

bool SdlAudioInit::pImpl::initialized = false;


SdlAudioInit::SdlAudioInit(void) : pimpl(new pImpl) {
  if (pimpl->initialized == false) {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
      // ƒƒO‹L˜^
      LogManager* log = LogManager::getObject();
      std::string message = "SDL_InitSubSystem(SDL_INIT_AUDIO): " +
        std::string(SDL_GetError());
      log->write(LogManager::Error, message.c_str());
    }
    atexit(SDL_Quit);
    pimpl->initialized = true;
  }
}


SdlAudioInit::~SdlAudioInit(void) {
}


bool SdlAudioInit::isInitialized(void) {
  return pimpl->initialized;
}
