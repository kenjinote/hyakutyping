/*!
  \brief 音量スライダー

  \author Satofumi KAMIMURA

  $Id$

  \todo いずれ、SliderComponent の特殊実装にしてしまう
  \todo ドラッグで選択位置を変更できるようにする
*/

#include "VolumeSliderComponent.h"
#include "Component.h"
#include "ColorSurface.h"
#include "DrawPanelSurface.h"
#include "EvaluateArea.h"
#include "InputEvent.h"
#include "GuiColors.h"
#include "SdlUtils.h"

using namespace beego;


struct VolumeSliderComponent::pImpl {

  enum {
    SliderWidth = 8,
    SliderSurface = 0,
    BackSurface,
    LastIndex,
  };

  size_t width_;
  size_t height_;

  SDL_Rect position_;
  bool position_changed_;

  bool input_enable_;
  int value_min_;
  int value_max_;
  int value_;
  bool value_changed_;

  Surface slider_;
  boost::shared_ptr<DrawPanelSurface> back_;
  DrawSurface draw_surfaces_[LastIndex];

  pImpl(size_t width, size_t height)
    : width_(width), height_(height), position_changed_(false),
      input_enable_(true), value_min_(0), value_max_(100), value_(value_min_),
      value_changed_(false),
      slider_(new ColorSurface(SliderWidth, height_, White)),
      back_(new DrawPanelSurface(width_, height_)) {

    // !!! とりあえず、灰色で塗りつぶしてみる
    back_->setBackgroundColor(Gray8);

    set_SdlRect(&position_, 0, 0, width_, height_);
  }

  void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                      size_t ticks) {

    // スライダーの描画
    size_t x = (width_ - SliderWidth) * value_ / value_max_;
    SDL_Rect draw_rect;
    set_SdlRect(&draw_rect,
                position_.x + x, position_.y,
                slider_->getWidth(), slider_->getHeight());
    updateDrawSurface(draw_surfaces_[SliderSurface],
                      slider_, area, ticks,
                      draw_rect, position_changed_);
    surfaces.push_back(&draw_surfaces_[SliderSurface]);

    // バーの描画
    updateDrawSurface(draw_surfaces_[BackSurface],
                      back_, area, ticks,
                      position_, position_changed_);
    surfaces.push_back(&draw_surfaces_[BackSurface]);

    position_changed_ = false;
  }

  void applyInput(const InputEvent& event,
                  std::vector<SDL_Rect>& applied_rects) {

    if (! input_enable_) {
      return;
    }

    // 左右のカーソル入力で選択を移動させる
    // !!! 押下中に移動するように変更する
    for (std::vector<SDLKey>::const_iterator it = event.key_released.begin();
         it != event.key_released.end(); ++it) {

      if ((value_ > value_min_) && (*it == SDLK_LEFT)) {
        // 左へ
        --value_;
        value_changed_ = true;
        position_changed_ = true;

      } else if ((value_ < value_max_) && (*it == SDLK_RIGHT)) {
        // 右へ
        ++value_;
        value_changed_ = true;
        position_changed_ = true;
      }
    }

    if (event.left_pressing) {

      SDL_Rect surface_area;
      set_SdlRect(&surface_area, position_.x, position_.y, width_, height_);
      bool cursor_inside = isPointInside(event.mx, event.my, surface_area);
      if (cursor_inside) {
        // 指定位置へ
        int x = event.mx - position_.x;
        if (x <= SliderWidth/2) {
          value_ = value_min_;
        } else if (x >= (static_cast<int>(width_) - SliderWidth/2)) {
          value_ = value_max_;
        } else {
          value_ = value_max_ * (x - SliderWidth/2) / (width_ - SliderWidth);
        }
        value_changed_ = true;
        position_changed_ = true;
      }
    }
  }
};


VolumeSliderComponent::VolumeSliderComponent(size_t width, size_t height)
  : pimpl(new pImpl(width, height)) {
}


VolumeSliderComponent::~VolumeSliderComponent(void) {
}


void VolumeSliderComponent::addSurfaceList(SurfaceList& surfaces,
                                           const SDL_Rect* area,
                                           size_t ticks) {

  pimpl->addSurfaceList(surfaces, area, ticks);
}


void VolumeSliderComponent::setPosition(const SDL_Rect* position) {

  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void VolumeSliderComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t VolumeSliderComponent::getWidth(void) {
  return pimpl->width_;
}


size_t VolumeSliderComponent::getHeight(void) {
  return pimpl->height_;
}


void VolumeSliderComponent::applyInput(const InputEvent& event,
                                       std::vector<SDL_Rect>& applied_rects) {

  pimpl->applyInput(event, applied_rects);
}


void VolumeSliderComponent::registerLayer(Layer* layer) {
  // !!!
}


int VolumeSliderComponent::getValue(void) {
  return pimpl->value_;
}


void VolumeSliderComponent::setValue(int value) {

  pimpl->value_ = value;

  if (pimpl->value_ < pimpl->value_min_) {
    pimpl->value_ = pimpl->value_min_;

  } else if (pimpl->value_ > pimpl->value_max_) {
    pimpl->value_ = pimpl->value_max_;
  }
}


bool VolumeSliderComponent::isChanged(void) {

  bool ret = pimpl->value_changed_;
  pimpl->value_changed_ = false;

  return ret;
}
