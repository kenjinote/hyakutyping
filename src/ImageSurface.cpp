/*!
  \file
  \brief 画像サーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ImageSurface.h"
#include "SdlSurface.h"
#include "LogManager.h"
#include <SDL_image.h>
#include <string>

using namespace beego;


struct ImageSurface::pImpl {
  SdlSurface surface;

  pImpl(const char* fname, bool transparent)
    : surface(IMG_Load(fname)) {
    if (surface.isInvalid()) {
      LogManager* log = LogManager::getObject();
      std::string message = "ImageSurface(): " + std::string(IMG_GetError());
      log->write(LogManager::Error, message.c_str());
      return;
    }
    if (transparent) {
      surface.transparent();
    }
  }
};


ImageSurface::ImageSurface(const char* imageFile, bool transparent)
  : pimpl(new pImpl(imageFile, transparent)) {
}


ImageSurface::~ImageSurface(void) {
}


void ImageSurface::draw(std::vector<SDL_Rect>& update_rects,
                        const SDL_Rect* pos, const SDL_Rect* area) {
  pimpl->surface.draw(update_rects, pos, area);
}


size_t ImageSurface::getWidth(void) {
  return pimpl->surface.getWidth();
}


size_t ImageSurface::getHeight(void) {
  return pimpl->surface.getHeight();
}


void ImageSurface::forceSetChanged(void) {
  pimpl->surface.forceSetChanged();
}


bool ImageSurface::isChanged(size_t ticks) {
  return pimpl->surface.isChanged(ticks);
}


bool ImageSurface::isTransparent(void) {
  return pimpl->surface.isTransparent();
}
