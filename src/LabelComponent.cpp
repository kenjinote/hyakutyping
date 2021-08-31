/*!
  \file
  \brief Label �R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$
*/

#include "LabelComponent.h"
#include "SurfaceInterface.h"
#include "Component.h"
#include "SdlUtils.h"

using namespace beego;


/*!
  \brief LabelComponent �̓����N���X
*/
struct LabelComponent::pImpl {
  Surface surface_;
  SDL_Rect position_;
  bool position_changed_;
  DrawSurface draw_surface_;

  pImpl(Surface surface) : surface_(surface), position_changed_(false) {
    set_SdlRect(&position_, 0, 0,
                static_cast<int>(surface_->getWidth()),
                static_cast<int>(surface_->getHeight()));
  }
};


LabelComponent::LabelComponent(Surface surface) : pimpl(new pImpl(surface)) {
}


LabelComponent::~LabelComponent(void) {
}


void LabelComponent::addSurfaceList(SurfaceList& surfaces,
                                    const SDL_Rect* area, size_t ticks) {

  updateDrawSurface(pimpl->draw_surface_, pimpl->surface_, area, ticks,
                    pimpl->position_, pimpl->position_changed_);
  surfaces.push_back(&pimpl->draw_surface_);
  pimpl->position_changed_ = false;
}


void LabelComponent::setPosition(const SDL_Rect* position) {

  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void LabelComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t LabelComponent::getWidth(void) {
  return pimpl->position_.w;
}


size_t LabelComponent::getHeight(void) {
  return pimpl->position_.h;
}


void LabelComponent::applyInput(const InputEvent& event,
                                std::vector<SDL_Rect>& applied_rects) {
  // �������Ȃ�
}


void LabelComponent::registerLayer(Layer* layer) {
  // !!!
}
