/*!
  \file
  \brief チェックボックス

  \author Satofumi KAMIMURA

  $Id$

  \todo チェック画像の下の透明な部分を、削る
*/

#include "CheckBoxComponent.h"
#include "check.h"
#include "Layer.h"
#include "Component.h"
#include "CreateSurfaceFromArray.h"
#include "SdlSurface.h"
#include "ColorSurface.h"
#include "GuiColors.h"
#include "InputEvent.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"

using namespace beego;


struct CheckBoxComponent::pImpl {
  enum {
    DrawCheck = 0,
    DrawInner,
    DrawOuter,
    DrawAdditionalSurface,
    DrawIndexMax,

    BoxSize = 14,
    LineWidth = 2,
  };

  bool is_checked_;
  bool is_check_changed_;
  size_t width_;
  size_t height_;
  size_t last_ticks_;
  SDL_Rect position_;
  bool position_changed_;
  Layer* layer_;
  std::vector<Surface> surface_;
  size_t line_width_;
  size_t outer_size_;
  size_t inner_size_;
  Surface check_surface_;
  Surface outer_surface_;
  Surface inner_surface_;
  size_t box_x_offset_;
  size_t box_y_offset_;
  size_t surface_x_offset_;
  size_t surface_y_offset_;
  DrawSurface draw_surfaces_[DrawIndexMax];

  pImpl(void)
    : is_checked_(false), is_check_changed_(false),
      width_(BoxSize), height_(BoxSize), last_ticks_(0),
      position_changed_(false), layer_(NULL), line_width_(LineWidth),
      outer_size_(BoxSize), inner_size_(outer_size_ - (LineWidth * 2)),
      check_surface_(new SdlSurface(createSurface(check,
                                                  check_width, check_height),
                                    true)),
      outer_surface_(new ColorSurface(outer_size_, outer_size_, Gray12)),
      inner_surface_(new ColorSurface(inner_size_, inner_size_, Black)),
      box_x_offset_(0), box_y_offset_(0),
      surface_x_offset_(0), surface_y_offset_(0) {

    set_SdlRect(&position_, 0, 0,
                static_cast<int>(outer_size_), static_cast<int>(outer_size_));
  }

  void addSurfaceList(SurfaceList& surfaces,
                      const SDL_Rect* area, size_t ticks) {

    SDL_Rect draw_area;
    last_ticks_ = ticks;

    // !!! このあたりの重複は、なんとかならんかな...

    // チェック画像
    if (is_checked_) {
      set_SdlRect(&draw_area, position_.x,
                  static_cast<int>(position_.y + box_y_offset_),
                  check_width, check_height);
      updateDrawSurface(draw_surfaces_[DrawCheck], check_surface_, area, ticks,
                        draw_area, position_changed_);
      surfaces.push_back(&draw_surfaces_[DrawCheck]);
    }

    // 内側の四角
    size_t inner_offset = line_width_;
    set_SdlRect(&draw_area,
                position_.x + inner_offset,
                position_.y + inner_offset + box_y_offset_,
                inner_size_, inner_size_);
    updateDrawSurface(draw_surfaces_[DrawInner], inner_surface_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[DrawInner]);

    // 外側の四角
    set_SdlRect(&draw_area, position_.x, position_.y + box_y_offset_,
                outer_size_, outer_size_);
    updateDrawSurface(draw_surfaces_[DrawOuter], outer_surface_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[DrawOuter]);

    // サーフェス
    if (! surface_.empty()) {
      set_SdlRect(&draw_area,
                  position_.x + surface_x_offset_,
                  position_.y + surface_y_offset_,
                  surface_[0]->getWidth(), surface_[0]->getHeight());
      updateDrawSurface(draw_surfaces_[DrawAdditionalSurface],
                        surface_[0], area, ticks,
                        draw_area, position_changed_);
      surfaces.push_back(&draw_surfaces_[DrawAdditionalSurface]);
    }

    position_changed_ = false;
  }

  void updateSize(void) {

    // !!! とりあえず、outer, inner は固定として実装してしまう
    // !!! とりあえず、サーフェスは右に追加されるとして、実装してしまう

    if (! surface_.empty()) {
      width_ = surface_x_offset_ + surface_[0]->getWidth();
      height_ = surface_[0]->getHeight();

      box_x_offset_ = 0;
      box_y_offset_ = -surface_y_offset_;
    }
  }

  void applyInput(const InputEvent& event) {
    // 範囲内でクリックがリリースされたら、選択状態をトグルする
    if (event.left_released) {
      SDL_Rect area_rect;
      set_SdlRect(&area_rect,
                  position_.x + box_x_offset_,
                  position_.y + box_y_offset_,
                  outer_size_, outer_size_);
      bool cursor_inside = isPointInside(event.mx, event.my, area_rect);

      if (! surface_.empty()) {
        set_SdlRect(&area_rect,
                    position_.x + surface_x_offset_,
                    position_.y + surface_y_offset_,
                    surface_[0]->getWidth(), surface_[0]->getHeight());
        cursor_inside |= isPointInside(event.mx, event.my, area_rect);
      }

      if (cursor_inside) {
        toggleCheck();
      }
    }
  }

  void toggleCheck(void) {
    bool on = ! is_checked_;
    setCheck(on);
  }

  void setCheck(bool on) {

    if (on == is_checked_) {
      // 同じなら、何もしない
      return;
    }

    // 描画を変更
    if (is_checked_) {
      // 描画中の場合は、チェック画像を削除する
      SurfaceList remove_surfaces;
      SDL_Rect draw_area;
      set_SdlRect(&draw_area,
                  position_.x, position_.y + box_y_offset_,
                  check_width, check_height);
      updateDrawSurface(draw_surfaces_[DrawCheck], check_surface_,
                        &draw_area, last_ticks_, draw_area, true);
      remove_surfaces.push_back(&draw_surfaces_[DrawCheck]);
      layer_->removeSurfaces(remove_surfaces);
    }

    // 再描画を促す
    is_check_changed_ = true;
    position_changed_ = true;

    // 状態の反転
    is_checked_ = on;
  }
};


CheckBoxComponent::CheckBoxComponent(void) : pimpl(new pImpl) {
}


CheckBoxComponent::~CheckBoxComponent(void) {
}


void CheckBoxComponent::addSurfaceList(SurfaceList& surfaces,
                                       const SDL_Rect* area, size_t ticks) {
  pimpl->addSurfaceList(surfaces, area, ticks);
}


void CheckBoxComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void CheckBoxComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t CheckBoxComponent::getWidth(void) {
  return pimpl->width_;
}


size_t CheckBoxComponent::getHeight(void) {
  return pimpl->height_;
}


void CheckBoxComponent::applyInput(const InputEvent& event,
                                   std::vector<SDL_Rect>& applied_rects) {
  pimpl->applyInput(event);
}


void CheckBoxComponent::registerLayer(Layer* layer) {
  pimpl->layer_ = layer;
}


void CheckBoxComponent::setSurface(Surface surface,
                                   int x_offset, int y_offset) {
  pimpl->surface_x_offset_ = x_offset;
  pimpl->surface_y_offset_ = y_offset;

  if (pimpl->surface_.empty()) {
    pimpl->surface_.push_back(surface);
  } else {
    pimpl->surface_[0] = surface;
  }
  pimpl->updateSize();
}


bool CheckBoxComponent::isChecked(void) {
  return pimpl->is_checked_;
}


bool CheckBoxComponent::isCheckChanged(void) {
  bool ret = pimpl->is_check_changed_;
  pimpl->is_check_changed_ = false;
  return ret;
}


void CheckBoxComponent::setCheck(bool on) {
  pimpl->setCheck(on);
}


void CheckBoxComponent::toggleCheck(void) {
  pimpl->toggleCheck();
}
