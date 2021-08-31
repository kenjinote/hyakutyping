/*!
  \file
  \brief メニューコンポーネント

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ButtonComponent.h"
#include "SwitchSurface.h"
#include "GridTypes.h"
#include "SdlUtils.h"
#include "Component.h"
#include "InputEvent.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "TtfResource.h"

using namespace beego;


struct ButtonComponent::pImpl {
  enum {
    NormalSurface = 0,
    PressedSurface,
    FocusedSurface,
    LastSurface,
  };
  boost::shared_ptr<SwitchSurface> surface;
  SDL_Rect position;
  bool position_changed;
  bool is_decided;
  bool is_changed;
  DrawSurface draw_surface;

  pImpl(void) :
    surface(new SwitchSurface), position_changed(false), is_decided(false),
    is_changed(false) {

    set_SdlRect(&position, 0, 0, 0, 0);
  }

  void updateComponentSize(void) {
    position.w = static_cast<int>(surface->getWidth());
    position.h = static_cast<int>(surface->getHeight());
  }
};


ButtonComponent::ButtonComponent(void) : pimpl(new pImpl) {
}


ButtonComponent::~ButtonComponent(void) {
}


void ButtonComponent::addSurfaceList(SurfaceList& surfaces,
                                     const SDL_Rect* area, size_t ticks) {
  updateDrawSurface(pimpl->draw_surface, pimpl->surface, area, ticks,
                    pimpl->position, pimpl->position_changed);
  surfaces.push_back(&pimpl->draw_surface);
  pimpl->position_changed = false;
}


void ButtonComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position, position->x, position->y);
  pimpl->updateComponentSize();
  pimpl->position_changed = true;
}


void ButtonComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position;
}


size_t ButtonComponent::getWidth(void) {
  return pimpl->surface->getWidth();
}


size_t ButtonComponent::getHeight(void) {
  return pimpl->surface->getHeight();
}


void ButtonComponent::registerNormalSurface(Surface surface,
                                            int x_offset, int y_offset) {
  pimpl->surface->registerSurface(surface, x_offset, y_offset,
                                  pImpl::NormalSurface);
  // 通常ボタンは、登録されたらデフォルトで表示するように設定する
  showNormalSurface();
  pimpl->updateComponentSize();
}


void ButtonComponent::registerPressedSurface(Surface surface,
                                             int x_offset, int y_offset) {
  pimpl->surface->registerSurface(surface, x_offset, y_offset,
                                  pImpl::PressedSurface);

  // サーフェス大きさ登録のために、一時的にサーフェスを切り替える
  size_t now_index = pimpl->surface->getSurfaceIndex();
  pimpl->updateComponentSize();
  pimpl->surface->switchSurface(now_index);
}


void ButtonComponent::registerFocusedSurface(Surface surface,
                                             int x_offset, int y_offset) {
  pimpl->surface->registerSurface(surface, x_offset, y_offset,
                                  pImpl::FocusedSurface);

  // サーフェス大きさ登録のために、一時的にサーフェスを切り替える
  size_t now_index = pimpl->surface->getSurfaceIndex();
  pimpl->updateComponentSize();
  pimpl->surface->switchSurface(now_index);
}


Surface ButtonComponent::getNormalSurface(void) {
  return pimpl->surface->getSurface(pImpl::NormalSurface);
}


void ButtonComponent::applyInput(const InputEvent& event,
                                 std::vector<SDL_Rect>& applied_rects) {

  if (pimpl->is_decided) {
    // 既に押下状態ならば、状態を更新しない
    return;
  }

  SDL_Rect surface_area;
  set_SdlRect(&surface_area, pimpl->position.x, pimpl->position.y,
              static_cast<int>(getWidth()), static_cast<int>(getHeight()));

  // マウスが、表示サーフェス内にある場合の処理
  bool changed = false;
  // !!! 実際には描画されてない場合がありうるが、とりあえず気にしない
  if (((event.mx >= surface_area.x) &&
       (event.mx <= surface_area.x + surface_area.w)) &&
      ((event.my >= surface_area.y) &&
       (event.my <= surface_area.y + surface_area.h))) {
    changed = true;

    if (event.left_released) {
      // 決定された
      showPressedSurface();
      pimpl->is_decided = true;
      pimpl->is_changed = true;

    } else {
      // フォーカスあり
      showFocusedSurface();
    }
  }
  if (! changed) {
    // 重複設定の管理は、SwitchSurface 内で行っている
    showNormalSurface();
  }
}


void ButtonComponent::registerLayer(Layer* layer) {
  // 何もしない
}


void ButtonComponent::showNormalSurface(void) {
  pimpl->surface->switchSurface(pImpl::NormalSurface);
}


void ButtonComponent::showPressedSurface(void) {
  pimpl->surface->switchSurface(pImpl::PressedSurface);
}


void ButtonComponent::showFocusedSurface(void) {
  pimpl->surface->switchSurface(pImpl::FocusedSurface);
}


bool ButtonComponent::isChanged(void) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool ButtonComponent::isDecided(void) {
  return pimpl->is_decided;
}


void ButtonComponent::setDecided(void) {
  pimpl->is_decided = true;
  pimpl->is_changed = true;
  showPressedSurface();
}


void ButtonComponent::releaseDecided(void) {
  pimpl->is_decided = false;
}


void ButtonComponent::createButton(boost::shared_ptr<ButtonComponent>& button,
                                   const TextProperty& normal_property,
                                   const TextProperty& selected_property,
                                   const TextProperty& pressed_property) {

  // 通常サーフェスの作成
  Surface normal(new TextSurface(normal_property));
  button->registerNormalSurface(normal);

  // 選択時サーフェスの作成
  Surface focused(new TextSurface(selected_property));
  button->registerFocusedSurface(focused);

  // 決定時サーフェスの作成
  Surface pressed(new TextSurface(pressed_property));
  button->registerPressedSurface(pressed);
}
