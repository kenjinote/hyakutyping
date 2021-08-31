/*!
  \file
  \brief 表示を入れ替え可能な LabelComponent

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SwitchLabelComponent.h"
#include "SwitchSurface.h"
#include "Component.h"
#include "SdlUtils.h"

using namespace beego;


struct SwitchLabelComponent::pImpl {
  Surface surface_;
  int mode_;
  size_t width_;
  size_t height_;
  SDL_Rect position_;
  bool position_changed_;
  DrawSurface draw_surface_;

  pImpl(boost::shared_ptr<SwitchSurface> surface, int mode)
    : surface_(surface), mode_(mode),
      width_(0), height_(0), position_changed_(false) {

    // 最大の幅、高さをこのコンポーネントの領域とする
    // !!! index が順番にあると仮定している
    size_t n = surface->size();
    for (size_t i = 0; i < n; ++i) {

      surface->switchSurface(i);
      size_t w = surface->getWidth();
      size_t h = surface->getHeight();

      if (width_ < w) {
        width_ = w;
      }
      if (height_ < h) {
        height_ = h;
      }
    }
    set_SdlRect(&position_, 0, 0, width_, height_);
  }
};


SwitchLabelComponent::SwitchLabelComponent(boost::shared_ptr<SwitchSurface>
                                           surface, int mode)
  : pimpl(new pImpl(surface, mode)) {
}


SwitchLabelComponent::~SwitchLabelComponent(void) {
}


void SwitchLabelComponent::addSurfaceList(SurfaceList& surfaces,
                                          const SDL_Rect* area, size_t ticks) {

  SDL_Rect draw_position = pimpl->position_;
  if (pimpl->mode_ & Center) {
    size_t w = pimpl->surface_->getWidth();
    draw_position.x += (pimpl->width_ - w) / 2;
  }
  updateDrawSurface(pimpl->draw_surface_, pimpl->surface_, area, ticks,
                    draw_position, pimpl->position_changed_);
  surfaces.push_back(&pimpl->draw_surface_);
  pimpl->position_changed_ = false;
}


void SwitchLabelComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void SwitchLabelComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t SwitchLabelComponent::getWidth(void) {
  return pimpl->width_;
}


size_t SwitchLabelComponent::getHeight(void) {
  return pimpl->height_;
}


void SwitchLabelComponent::applyInput(const InputEvent& event,
                                      std::vector<SDL_Rect>& applied_rects) {
  // 実装しない
}


void SwitchLabelComponent::registerLayer(Layer* layer) {
  // !!!
}
