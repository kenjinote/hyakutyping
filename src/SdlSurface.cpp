/*!
  \file
  \brief SDL サーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlSurface.h"
#include "ModifySurface.h"
#include "SdlUtils.h"
#include "LogManager.h"
#include <string>

using namespace beego;


/*!
  \brief SdlSurface の内部クラス
*/
struct SdlSurface::pImpl {
  SDL_Surface* scr;
  SDL_Surface* surface;
  bool is_changed;
  bool is_transparent;
  size_t now_percent;

  pImpl(SDL_Surface* surface_, bool transparent_on)
    : scr(SDL_GetVideoSurface()), surface(surface_), is_changed(false),
      is_transparent(false), now_percent(100) {

    if (scr == NULL) {
      LogManager* log = LogManager::getObject();
      std::string message =
        "SdlSurface::pImpl(): Screen surface must be created.";
      log->write(LogManager::Warning, message.c_str());
      return;
    }
    if (transparent_on) {
      transparent();
    }
  }

  ~pImpl(void) {
    if (surface) {
      SDL_FreeSurface(surface);
    }
  }

  void transparent(void) {
    SDL_Surface* modified = beego::transparent(scr, surface);
    if (modified) {
      surface = modified;
      is_transparent = true;
    }
  }
};


SdlSurface::SdlSurface(SDL_Surface* surface, bool transparent)
  : pimpl(new pImpl(surface, transparent)) {
}


SdlSurface::~SdlSurface(void) {
}


bool SdlSurface::isInvalid(void) {
  return (pimpl->surface == NULL) ? true : false;
}


void SdlSurface::transparent(void) {
  pimpl->transparent();
}


void SdlSurface::draw(std::vector<SDL_Rect>& update_rects,
                      const SDL_Rect* pos, const SDL_Rect* area) {

  if ((pimpl->scr == NULL) &&
      ((pimpl->scr = SDL_GetVideoSurface()) == NULL)) {
    return;
  }

  SDL_BlitSurface(pimpl->surface, const_cast<SDL_Rect*>(area),
                  pimpl->scr, const_cast<SDL_Rect*>(pos));
  update_rects.push_back(*pos);
}


size_t SdlSurface::getWidth(void) {
  return (pimpl->surface == NULL) ? 0 : pimpl->surface->w;
}


size_t SdlSurface::getHeight(void) {
  return (pimpl->surface == NULL) ? 0 : pimpl->surface->h;
}


void SdlSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool SdlSurface::isChanged(size_t ticks) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool SdlSurface::isTransparent(void) {
  return pimpl->is_transparent;
}


void SdlSurface::setAlpha(size_t percent) {

  size_t modified_percent = beego::setAlpha(percent,
                                            pimpl->now_percent,
                                            pimpl->surface);

  if (modified_percent != pimpl->now_percent) {
    pimpl->now_percent = modified_percent;
    pimpl->is_changed = true;
    pimpl->is_transparent = (percent >= 100) ? false : true;
  }
}


SDL_Surface* SdlSurface::getRawSurface(void) {
  return pimpl->surface;
}
