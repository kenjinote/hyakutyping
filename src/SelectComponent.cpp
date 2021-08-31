/*!
  \file
  \brief 水平セレクタ

  \author Satofumi KAMIMURA

  $Id$

  \todo decided_index_ を is_decided_ に置き換える
*/

#include "SelectComponent.h"
#include "TtfResource.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "SwitchSurface.h"
#include "Component.h"
#include "InputEvent.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"

using namespace beego;


struct SelectComponent::pImpl {
  enum {
    Normal = 0,
    Active,
  };
  SDL_Rect position;
  bool position_changed;
  TtfResource* font;
  size_t font_size;
  Uint32 fore_active;
  Uint32 fore_normal;
  Uint32 back;
  bool is_transparent;
  SelectMode mode;
  typedef std::vector<boost::shared_ptr<SwitchSurface> > ItemSurfaces;
  ItemSurfaces surfaces;
  std::vector<DrawSurface> draw_surfaces;
  size_t items_offset;
  int now_index_;
  int decided_index_;
  bool key_changed_;
  bool input_enable;

  pImpl(const TextProperty& text_property,
        Uint32 active_color, SelectMode mode_)
    : position_changed(false),
      font(text_property.font), font_size(text_property.size),
      fore_active(active_color), fore_normal(text_property.fore_color),
      back(text_property.back_color),
      is_transparent(text_property.transparent),
      mode(mode_), items_offset(0), now_index_(NotSelected),
      decided_index_(NotSelected), key_changed_(false), input_enable(true) {

    set_SdlRect(&position, 0, 0, 0, 0);
  }

  void updateSize(void) {

    size_t total_width = 0;
    size_t max_height = 0;

    for (ItemSurfaces::iterator it = surfaces.begin();
         it != surfaces.end(); ++it) {

      size_t width = (*it)->getWidth();
      total_width += width + items_offset;

      size_t height = (*it)->getHeight();
      if (height > max_height) {
        max_height = height;
      }
    }
    total_width -= items_offset;

    position.w = total_width;
    position.h = max_height;

    DrawSurface dummy;
    draw_surfaces.push_back(dummy);
  }

  void changeSelector(int next_index) {

    if (now_index_ >= 0) {
      // サーフェスを通常サーフェスに戻す
      surfaces[now_index_]->switchSurface(pImpl::Normal);
    }

    // サーフェスの変更
    if (next_index >= 0) {
      // サーフェスを変更し、再描画を促す
      surfaces[next_index]->switchSurface(pImpl::Active);
    }

    now_index_ = next_index;
    position_changed = true;
  }

  void createItem(TextProperty& property) {

    Surface normal_surface(new TextSurface(property));

    property.back_color = fore_active;
    property.transparent = false;
    Surface active_surface(new TextSurface(property));

    boost::shared_ptr<SwitchSurface> switch_surface(new SwitchSurface);
    switch_surface->registerSurface(normal_surface, 0, 0, pImpl::Normal);
    switch_surface->registerSurface(active_surface, 0, 0, pImpl::Active);
    switch_surface->switchSurface(0);

    surfaces.push_back(switch_surface);
    updateSize();
  }
};


SelectComponent::SelectComponent(const TextProperty&
                                                     text_property,
                                                     Uint32 active_color,
                                                     SelectMode mode)
  : pimpl(new pImpl(text_property, active_color, mode)) {
}


SelectComponent::~SelectComponent(void) {
}


void SelectComponent::addSurfaceList(SurfaceList& surfaces,
                                               const SDL_Rect* area,
                                               size_t ticks) {

  SDL_Rect surface_position = pimpl->position;
  size_t index = 0;
  for (pImpl::ItemSurfaces::iterator it = pimpl->surfaces.begin();
       it != pimpl->surfaces.end(); ++it, ++index) {

    size_t width = (*it)->getWidth();
    size_t height = (*it)->getHeight();

    surface_position.w = width;
    surface_position.h = height;

    updateDrawSurface(pimpl->draw_surfaces[index], *it, area, ticks,
                      surface_position, pimpl->position_changed);
    surfaces.push_back(&pimpl->draw_surfaces[index]);

    surface_position.x += width + pimpl->items_offset;
  }
  pimpl->position_changed = false;
}


void SelectComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position, position->x, position->y,
              pimpl->position.w, pimpl->position.h);
  pimpl->position_changed = true;
}


void SelectComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position;
}


size_t SelectComponent::getWidth(void) {
  return pimpl->position.w;
}


size_t SelectComponent::getHeight(void) {
  return pimpl->position.h;
}


void SelectComponent::applyInput(const InputEvent& event,
                                           std::vector<SDL_Rect>&
                                           applied_rects) {
  if ((! pimpl->input_enable) || (pimpl->decided_index_ != NotSelected)) {
    return;
  }

  // 左右のカーソル入力で選択を移動させる
  for (std::vector<SDLKey>::const_iterator it = event.key_released.begin();
       it != event.key_released.end(); ++it) {

    int max = pimpl->surfaces.size();
    if ((pimpl->now_index_ > 0) && (*it == SDLK_LEFT)) {
      // 左へ
      pimpl->changeSelector(pimpl->now_index_ - 1);
      pimpl->key_changed_ = true;

    } else if ((pimpl->now_index_ < (max - 1)) &&  (*it == SDLK_RIGHT)) {
      // 右へ
      pimpl->changeSelector(pimpl->now_index_ + 1);
      pimpl->key_changed_ = true;

    } else if (*it == SDLK_RETURN) {
      // リターンキーで決定
      pimpl->decided_index_ = pimpl->now_index_;
      break;
    }
  }

  if (event.left_released && (event.mouse_moved || (! pimpl->key_changed_))) {
    pimpl->key_changed_ = false;

    // クリックされた場所に選択を移動させる
    int index = 0;
    SDL_Rect surface_area = pimpl->position;
    for (pImpl::ItemSurfaces::iterator it = pimpl->surfaces.begin();
         it != pimpl->surfaces.end(); ++it, ++index) {

      size_t width = (*it)->getWidth();
      size_t height = (*it)->getHeight();

      surface_area.w = width;
      surface_area.h = height;

      bool cursor_inside = isPointInside(event.mx, event.my, surface_area);
      if (cursor_inside) {
        pimpl->decided_index_ = index;
        pimpl->changeSelector(index);
      }
      surface_area.x += width + pimpl->items_offset;
    }
  }
}


void SelectComponent::registerLayer(Layer* layer) {
}


void SelectComponent::addItem(const char* text) {

  TextProperty property(pimpl->font, text, pimpl->font_size,
                        pimpl->fore_normal, pimpl->back, pimpl->is_transparent);
  pimpl->createItem(property);
}


void SelectComponent::addItem(const Uint16* utext) {

  TextProperty property(pimpl->font, utext, pimpl->font_size,
                        pimpl->fore_normal, pimpl->back, pimpl->is_transparent);
  pimpl->createItem(property);
}


void SelectComponent::setItemsOffset(int x_offset) {
  pimpl->items_offset = x_offset;
  pimpl->updateSize();
}


int SelectComponent::getSelected(void) {
  return pimpl->now_index_;
}


void SelectComponent::setSelected(int index) {

  if ((index < static_cast<int>(pimpl->surfaces.size())) &&
      (pimpl->now_index_ != index)) {
    pimpl->changeSelector(index);
  }
}


void SelectComponent::enableInput(void) {
  pimpl->input_enable = true;
}


void SelectComponent::disableInput(void) {
  pimpl->input_enable = false;
}


int SelectComponent::getDecided(void) {
  return pimpl->decided_index_;
}


void SelectComponent::releaseDecided(void) {
  pimpl->decided_index_ = NotSelected;
}
