/*!
  \file
  \brief SDL の画面操作

  \author Satofumi KAMIMURA

  $Id$

  \todo UTF8 キャプションを受け付けられるように変更する
*/

#include "SdlVideo.h"
#include "SdlVideoInit.h"
#include "LockGuard.h"
#include "LogManager.h"
#include "SdlSurface.h"
#include <string>

using namespace beego;
using namespace SDL;


/*!
  \brief SdlVideo の内部クラス

  \todo Windows にて SDL_Flip() の評価を行う
  \todo Windows にて show(), hide() を繰り返した際、show() の返り値が同じか評価
*/
struct SdlVideo::pImpl : private SdlVideoInit {
  std::string error_message_;
  SDL_mutex* mutex_;
  bool initialized_;
  std::string caption_;

  enum {
    DefaultWidth = 640,
    DefaultHeight = 480,
    DefaultBpp = 0,             //!< デフォルト深度を利用
  };
  size_t now_w_;
  size_t now_h_;
  size_t next_bpp_;
  size_t next_w_;
  size_t next_h_;
  bool fullscreen_;

  SDL_Surface* icon_surface_;
  Uint8* icon_mask_;

  pImpl(void)
    : error_message_("no error."),
      mutex_(SDL_CreateMutex()), initialized_(false), caption_(""),
      now_w_(0), now_h_(0), next_bpp_(DefaultBpp),
      next_w_(DefaultWidth), next_h_(DefaultHeight), fullscreen_(false),
      icon_surface_(NULL), icon_mask_(NULL) {
  }

  ~pImpl(void) {
    SDL_DestroyMutex(mutex_);
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }
};


SdlVideo::SdlVideo(void) : pimpl(pImpl::getObject()) {
}


SdlVideo::~SdlVideo(void) {
  hide();
}


const char* SdlVideo::what(void) {
  return pimpl->error_message_.c_str();
}


void SdlVideo::setFullscreenMode(bool on) {
  pimpl->fullscreen_ = on;
}


bool SdlVideo::isFullscreen(void) {
  return pimpl->fullscreen_;
}


SDL_Surface* SdlVideo::show(Uint32 mode) {
  LockGuard guard(pimpl->mutex_);

  if (pimpl->initialized_) {
    return SDL_GetVideoSurface();
  }

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    LogManager* log = LogManager::getObject();
    std::string message = "SdlVideo::show(): " + std::string(SDL_GetError());
    log->write(LogManager::Error, message.c_str());

    return NULL;
  }

  // アイコンの適用
  if (pimpl->icon_surface_) {
    SDL_WM_SetIcon(pimpl->icon_surface_, pimpl->icon_mask_);
  }

  // サーフェスに関する記述がなければ、SDL_SWSURFACE モードで動作させる
  mode = (mode == 0) ? SDL_SWSURFACE : mode;
  mode |= pimpl->fullscreen_ ? SDL_FULLSCREEN : 0;

  // !!! Windows 環境で、SDL_Flip() の評価を行う

  SDL_Surface* scr =
    SDL_SetVideoMode(static_cast<int>(pimpl->next_w_),
                     static_cast<int>(pimpl->next_h_),
                     static_cast<int>(pimpl->next_bpp_), mode);
  if (scr == NULL) {
    pimpl->error_message_ = SDL_GetError();
  }
  pimpl->initialized_ = (scr == NULL) ? false : true;

  if (pimpl->initialized_ != false) {
    pimpl->now_w_ = pimpl->next_w_;
    pimpl->now_h_ = pimpl->next_h_;
    SDL_WM_SetCaption(pimpl->caption_.c_str(), NULL);
  }

  return scr;
}


void SdlVideo::hide(void) {
  if (pimpl->initialized_) {
    LockGuard guard(pimpl->mutex_);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    pimpl->initialized_ = false;
    pimpl->now_w_ = 0;
    pimpl->now_h_ = 0;
  }
}


void SdlVideo::setCaption(const char* caption) {
  pimpl->caption_ = caption;
}


void SdlVideo::setIcon(SDL_Surface* icon_surface, Uint8* mask) {
  pimpl->icon_surface_ = icon_surface;
  pimpl->icon_mask_ = mask;
}


void SdlVideo::setWindowBpp(size_t bpp) {
  pimpl->next_bpp_ = bpp;
}


void SdlVideo::setWindowSize(size_t width, size_t height) {
  pimpl->next_w_ = width;
  pimpl->next_h_ = height;
}


size_t SdlVideo::getWidth(void) {
  return pimpl->now_w_;
}


size_t SdlVideo::getHeight(void) {
  return pimpl->now_h_;
}
