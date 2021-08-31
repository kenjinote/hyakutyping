/*!
  \file
  \brief SDL の初期化クラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlInit.h"
#include "LogManager.h"
#include <SDL.h>
#include <stdlib.h>
#include <string>

using namespace beego;


/*!
  \brief SdlInit の内部クラス
*/
struct SdlInit::pImpl {
  static bool initialized;
};

bool SdlInit::pImpl::initialized = false;


SdlInit::SdlInit(void) : pimpl(new pImpl) {

  // !!! 問題が発生するようであれば、double lock にする

  if (pimpl->initialized == false) {
    if (SDL_Init(0) < 0) {
      // ログ記録
      LogManager* log = LogManager::getObject();
      std::string message = "SDL_Init(): " + std::string(SDL_GetError());
      log->write(LogManager::Error, message.c_str());
      throw;
    }
    atexit(SDL_Quit);
    pimpl->initialized = true;
  }
}


SdlInit::~SdlInit(void) {
}
