/*!
  \file
  \brief オプション用メニューコンポーネント

  \author Satofumi KAMIMURA

  $Id$
*/

#include "OptionMenuComponent.h"
#include "TextSurface.h"
#include "RedrawSurface.h"
#include "InputEvent.h"
#include "GridTypes.h"
#include "SdlUtils.h"

using namespace beego;


struct OptionMenuComponent::pImpl {
  size_t width_;
  size_t height_;

  SDL_Rect position_;
  bool position_changed_;

  bool icon_placed_;
  Component icon_component_;
  Grid<int> icon_offset_;
  int icon_place_mode_;
  size_t icon_pre_selected_;
  size_t icon_width_;
  size_t icon_height_;
  SDL_Rect pre_icon_position_;

  size_t menu_index_;
  bool key_changed_;

  std::vector<Surface> titles_;
  std::vector<DrawSurface> draw_surfaces_;
  DrawSurface draw_surface_;
  boost::shared_ptr<RedrawSurface> redraw_surface_;
  std::vector<Grid<int> > component_offset_;
  std::vector<Grid<int> > items_offset_;
  std::vector<Component> components_;
  std::vector<Grid<int> > components_position_;

  pImpl(void) : width_(0), height_(0), position_changed_(false),
                icon_placed_(false), icon_place_mode_(None),
                icon_pre_selected_(1), icon_width_(0), icon_height_(0),
                menu_index_(0), key_changed_(false),
                redraw_surface_(new RedrawSurface) {

    set_SdlRect(&position_, 0, 0, 0, 0);
    set_SdlRect(&pre_icon_position_, 0, 0, 0, 0);
  }

  void clear(void) {
    width_ = 0;
    height_ = 0;
    titles_.clear();
    draw_surfaces_.clear();
    component_offset_.clear();
    items_offset_.clear();
    components_.clear();
    components_position_.clear();
  }

  void updateComponentSize(void) {

    size_t max_w = 0;
    size_t max_h = 0;

    // !!! PlaceOption は、どこで反映させるべきなのかな？

    // 項目名におけるサイズ更新
    size_t index = 0;
    for (std::vector<Surface>::iterator it = titles_.begin();
         it != titles_.end(); ++it, ++index) {

      // 最大サイズの伸張
      // !!! 本来は、titles_ の方のサイズも考慮すべき
      size_t w = component_offset_[index].x + components_[index]->getWidth();
      if (w > max_w) {
        max_w = w;
      }
      size_t h = component_offset_[index].y + components_[index]->getHeight();
      if (h > max_h) {
        max_h = h;
      }
    }

    width_ = max_w;
    height_ = max_h;
  }

  void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                      size_t ticks) {

    // 項目の配置
    SDL_Rect icon_pos;
    size_t x = 0;
    size_t y = 0;
    size_t index = 0;
    for (std::vector<Surface>::iterator it = titles_.begin();
         it != titles_.end(); ++it, ++index) {

      SDL_Rect title_position;
      set_SdlRect(&title_position, position_.x + x, position_.y + y,
                  width_, height_);
      updateDrawSurface(draw_surfaces_[index], titles_[index], area, ticks,
                        title_position, position_changed_);
      surfaces.push_back(&draw_surfaces_[index]);

      if (menu_index_ == index) {
        set_SdlRect(&icon_pos,
                    title_position.x + icon_offset_.x,
                    title_position.y + icon_offset_.y,
                    icon_width_, icon_height_);
      }

      // コンポーネントの配置
      components_[index]->addSurfaceList(surfaces, area, ticks);

      x += items_offset_[index].x;
      y += items_offset_[index].y;
    }

    // アイコンの描画
    if (icon_placed_) {
      // アイコンの配置
      if (icon_pre_selected_ != menu_index_) {
        icon_pre_selected_ = menu_index_;

        // 移動前の領域をクリア
        redraw_surface_->setRedrawRect(pre_icon_position_);
        updateDrawSurface(draw_surface_, redraw_surface_,
                          area, ticks, pre_icon_position_, true);
        surfaces.push_back(&draw_surface_);

        icon_component_->setPosition(&icon_pos);
        pre_icon_position_ = icon_pos;
      }
      icon_component_->addSurfaceList(surfaces, area, ticks);
    }

    position_changed_ = false;
  }

  void applyInput(const InputEvent& event,
                  std::vector<SDL_Rect>& applied_rects) {

    // 項目がなければ、戻る
    if (titles_.empty()) {
      return;
    }

    // キー入力の反映
    for (std::vector<SDLKey>::const_iterator it = event.key_released.begin();
         it != event.key_released.end(); ++it) {
      size_t index_max = titles_.size() -1;

      if ((menu_index_ > 0) && (*it == SDLK_UP)) {
        // 上へ
        --menu_index_;
        key_changed_ = true;

      } else if ((menu_index_ < index_max) &&  (*it == SDLK_DOWN)) {
        // 下へ
        ++menu_index_;
        key_changed_ = true;
      }

      components_[menu_index_]->applyInput(event, applied_rects);
    }

    // マウス入力の反映
    size_t y = 0;
    size_t index = 0;
    for (std::vector<Surface>::iterator it = titles_.begin();
         it != titles_.end(); ++it, ++index) {

      // !!! 気にせず入力を渡してみる
      // !!! ただし、入力が処理されたら、それ以降には処理を渡さないようにする
      // !!! applied_rects を判定に使う

      // !!! あと、キー入力を渡さない工夫が必要
      // !!!
      InputEvent mouseOnly_event = event;
      mouseOnly_event.key_pressed.clear();
      mouseOnly_event.key_released.clear();

      components_[index]->applyInput(mouseOnly_event, applied_rects);

      int y_top = position_.y + y;
      int y_bottom = y_top + (*it)->getHeight();
      if ((event.my >= y_top) && (event.my <= y_bottom)) {
        // マウスの Y 位置があるコンポーネントに入力を渡す
        //components_[index]->applyInput(event, applied_rects);
        menu_index_ = index;

        //break;
      }
      y += items_offset_[index].y;
    }
  }

  void setComponentsPosition(void) {

    // 項目の配置
    size_t x = 0;
    size_t y = 0;
    size_t index = 0;
    for (std::vector<Surface>::iterator it = titles_.begin();
         it != titles_.end(); ++it, ++index) {

      // コンポーネントの配置
      Component& p = components_[index];
      SDL_Rect component_rect;
      set_SdlRect(&component_rect,
                  position_.x + component_offset_[index].x + x,
                  position_.y + component_offset_[index].y + y,
                  p->getWidth(), p->getHeight());
      p->setPosition(&component_rect);

      x += items_offset_[index].x;
      y += items_offset_[index].y;
    }
  }
};


OptionMenuComponent::OptionMenuComponent(void) : pimpl(new pImpl) {
}


OptionMenuComponent::~OptionMenuComponent(void) {
}


void OptionMenuComponent::addSurfaceList(SurfaceList& surfaces,
                                         const SDL_Rect* area,
                                         size_t ticks) {
  pimpl->addSurfaceList(surfaces, area, ticks);
}


void OptionMenuComponent::setPosition(const SDL_Rect* position) {

  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->width_, pimpl->height_);
  pimpl->setComponentsPosition();

  pimpl->position_changed_ = true;
}


void OptionMenuComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t OptionMenuComponent::getWidth(void) {
  return pimpl->width_;
}


size_t OptionMenuComponent::getHeight(void) {
  return pimpl->height_;
}


void OptionMenuComponent::applyInput(const InputEvent& event,
                                     std::vector<SDL_Rect>& applied_rects) {
  pimpl->applyInput(event, applied_rects);
}


void OptionMenuComponent::registerLayer(Layer* layer) {

  for (std::vector<Component>::iterator it = pimpl->components_.begin();
       it != pimpl->components_.end(); ++it) {
    (*it)->registerLayer(layer);
  }
}


void OptionMenuComponent::setIconComponent(Component icon,
                                           int x_offset, int y_offset,
                                           int mode) {
  pimpl->icon_component_ = icon;
  pimpl->icon_placed_ = true;

  pimpl->icon_offset_.x = x_offset;
  pimpl->icon_offset_.y = y_offset;
  pimpl->icon_place_mode_ = mode;
  pimpl->icon_width_ = pimpl->icon_component_->getWidth();
  pimpl->icon_height_ = pimpl->icon_component_->getHeight();
}


void OptionMenuComponent::addItem(const TextProperty& title,
                                  boost::shared_ptr<ComponentInterface>
                                  component, int x_offset, int y_offset,
                                  int place_mode) {

  // 配置位置の調整
  // !!! place_mode まわり

  // 配置オフセットの登録
  pimpl->items_offset_.push_back(Grid<int>(0, 0));

  // 描画オフセットの登録
  pimpl->component_offset_.push_back(Grid<int>(x_offset, y_offset));

  // タイトルサーフェスの作成、登録
  Surface title_text(new TextSurface(title));
  pimpl->titles_.push_back(title_text);
  pimpl->draw_surfaces_.resize(pimpl->titles_.size());

  // コンポーネントの登録
  pimpl->components_.push_back(component);

  // 位置の再計算
  pimpl->updateComponentSize();
}


void OptionMenuComponent::setItemsOffset(int x_offset, int y_offset) {

  for (std::vector<Grid<int> >::iterator it = pimpl->items_offset_.begin();
       it != pimpl->items_offset_.end(); ++it) {

    it->x += x_offset;
    it->y += y_offset;
  }
  pimpl->updateComponentSize();
}


void OptionMenuComponent::addItemOffset(int x_offset, int y_offset) {

  if (pimpl->items_offset_.empty()) {
    // 項目が登録されていない場合は、無視する
    return;
  }

  Grid<int>& offset = pimpl->items_offset_.back();
  offset.x += x_offset;
  offset.y += y_offset;

  pimpl->updateComponentSize();
}
