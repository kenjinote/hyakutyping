/*!
  \file
  \brief レイヤー

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Layer.h"
#include "ComponentInterface.h"
#include "LayerManager.h"

using namespace beego;


/*!
  \brief Layer の内部クラス
*/
struct Layer::pImpl {
  ComponentList component_list;
  LayerManager* manager;
  bool enable;

  pImpl(void) : manager(NULL), enable(true) {
  }

  void positionTouch(Component component) {
    SDL_Rect position;
    component->getPosition(&position);
    component->setPosition(&position);
  }
};


Layer::Layer(void) : pimpl(new pImpl) {
}


Layer::~Layer(void) {
}


void Layer::addComponentList(ComponentList& components) const {

  for (ComponentList::iterator it = pimpl->component_list.begin();
       it != pimpl->component_list.end(); ++it) {
    components.push_back(*it);
  }
}


void Layer::addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                           size_t ticks) {

  // 保持している全 Component に対して addSurfaceList() を呼び出す
  for (ComponentList::iterator it = pimpl->component_list.begin();
       it != pimpl->component_list.end(); ++it) {

    (*it)->addSurfaceList(surfaces, area, ticks);
  }
}


/*!
  \attention 新規実装に対して未調整
*/
void Layer::push_front(Component component) {
  if (! pimpl->enable) {
    return;
  }

  component->registerLayer(this);
  pimpl->component_list.push_front(component);

  // 位置を更新し、再描画を促す
  // !!! あまり良くない実装
  // !!! registerLayer() の中で forceSetChanged() を呼び出すように変更すべき
  pimpl->positionTouch(component);
}


/*!
  \attention 新規実装に対して未調整
*/
void Layer::push_back(Component component) {
  if (! pimpl->enable) {
    return;
  }

  component->registerLayer(this);
  pimpl->component_list.push_back(component);

  // 位置を更新し、再描画を促す
  // !!! あまり良くない実装
  pimpl->positionTouch(component);
}


/*!
  \attention 新規実装に対して未調整
*/
void Layer::insert(const Component dst, Component component) {
  if (! pimpl->enable) {
    return;
  }

  component->registerLayer(this);
  ComponentList::iterator it = find(pimpl->component_list.begin(),
                                    pimpl->component_list.end(), dst);
  if (it != pimpl->component_list.end()) {
    pimpl->component_list.insert(it, component);
  }

  // 位置を更新し、再描画を促す
  // !!! あまり良くない実装
  pimpl->positionTouch(component);
}


void Layer::remove(Component component) {
  if (! pimpl->enable) {
    return;
  }

  ComponentList::iterator it = find(pimpl->component_list.begin(),
                                    pimpl->component_list.end(), component);
  if (it != pimpl->component_list.end()) {

    // 削除領域の取得
    SDL_Rect removed_rect;
    (*it)->getPosition(&removed_rect);
    //fprintf(stderr, "Layer::remove: %d, %d, %d, %d\n", removed_rect.x, removed_rect.y, removed_rect.w, removed_rect.h);

    // 削除サーフェスリストの評価
    SurfaceList remove_list;
    size_t dummy_ticks = 0;
    (*it)->addSurfaceList(remove_list, &removed_rect, dummy_ticks);
    pimpl->manager->removeSurface(remove_list);

    // コンポーネントの登録削除
    pimpl->component_list.erase(it);
  }
}


void Layer::removeSurfaces(SurfaceList& surfaces) {

  // 削除サーフェスリストの評価
  pimpl->manager->removeSurface(surfaces);
}


void Layer::registerManager(LayerManager* obj) {
  pimpl->manager = obj;
}


void Layer::enable(void) {
  pimpl->enable = true;
}


void Layer::disable(void) {
  pimpl->enable = false;
}
