/*!
  \file
  \brief レイヤー管理

  \author Satofumi KAMIMURA

  $Id$

  \todo LogManager まわりが SdlSurface と同じならばくくる
  \todo pimpl をシングルトンにすべき

  \attention なんか、内部のコンポーネント間で比較を行って、挙動がおかしい。つかったら、だめ
*/

#include "LayerManager.h"
#include "Layer.h"
#include "SurfaceInterface.h"
#include "ComponentInterface.h"
#include "GuiColors.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"
#include "LogManager.h"
#include <string>

using namespace beego;


struct LayerManager::pImpl {
  typedef std::vector<Layer*> LayerList;
  SurfaceList& surface_list;
  LayerList layer_list;
  SDL_Surface* scr;
  Uint32 back_color;
  size_t width;
  size_t height;
  typedef std::vector<SDL_Rect> Rects;
  Rects back_rects;
  Rects remove_update_rects;

  pImpl(SurfaceList& surfaces)
    : surface_list(surfaces), scr(SDL_GetVideoSurface()), width(0), height(0) {
#if 0
    if (scr == NULL) {
      LogManager* log = LogManager::getObject();
      std::string message =
        "LayerManager::pImpl(): Screen surface must be created.";
      log->write(LogManager::Warning, message.c_str());
      return;
    }
#endif
    updateScreenInfo();
  }

  void updateScreenInfo(void) {
    if (scr) {
      width = scr->w;
      height = scr->h;
      back_color = getSdlColor(scr, Black);
    }
  }
};


LayerManager::LayerManager(SurfaceList& surface_list)
  : pimpl(new pImpl(surface_list)) {
}


LayerManager::~LayerManager(void) {
}


void LayerManager::updateScreenSurface(SDL_Surface* scr) {
  pimpl->scr = scr;
  pimpl->updateScreenInfo();
}


void LayerManager::applyInput(ComponentList& components,
                              const InputEvent& event,
                              std::vector<SDL_Rect>& applied_rects) {

  SDL_Rect component_rect;
  for (ComponentList::iterator it = components.begin();
       it != components.end(); ++it) {
    (*it)->applyInput(event, applied_rects);

    // 入力判定を行った領域を登録
    (*it)->getPosition(&component_rect);
    component_rect.w = static_cast<int>((*it)->getWidth());
    component_rect.h = static_cast<int>((*it)->getHeight());
    applied_rects.push_back(component_rect);
  }
}


void LayerManager::redraw(std::vector<SDL_Rect>& update_rects,
                          SurfaceList& surfaces) {

  if (! pimpl->scr) {
    return;
  }

  // 削除サーフェス分の update_rects を受け取る
  // !!! 本来は、insert にするべき。いずれ修正する
  update_rects = pimpl->remove_update_rects;
  pimpl->remove_update_rects.clear();

  // 背景色での塗りつぶし
  //fprintf(stderr, "back_rects: %d\n", pimpl->back_rects.size());
  for (pImpl::Rects::iterator it = pimpl->back_rects.begin();
       it != pimpl->back_rects.end(); ++it) {
    SDL_FillRect(pimpl->scr, &*it, pimpl->back_color);
    update_rects.push_back(*it);
  }
  pimpl->back_rects.clear();

  for (SurfaceList::reverse_iterator rit = surfaces.rbegin();
       rit != surfaces.rend(); ++rit) {
    bool is_changed = (*rit)->is_changed | (*rit)->position_changed;

    if (is_changed) {
      //SDL_Rect& rect = (*rit)->position;
      //fprintf(stderr, "draw!!! (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
      (*rit)->surface->draw(update_rects, &(*rit)->position, &(*rit)->area);

      // !!! 次回の createSurfaceList() 評価時に削除されるのを防ぐため
      (*rit)->is_changed = false;
      continue;
    }
  }
}


void LayerManager::push_front(Layer* layer) {
  pimpl->layer_list.insert(pimpl->layer_list.begin(), layer);
  layer->registerManager(this);

  // !!! このときに、SurfaceList の更新も行う
}


void LayerManager::push_back(Layer* layer) {

  pimpl->layer_list.push_back(layer);
  layer->registerManager(this);

  // !!! このときに、SurfaceList の更新も行う
}


void LayerManager::insert(const Layer* dst, Layer* layer) {

  pImpl::LayerList::iterator it = find(pimpl->layer_list.begin(),
                                       pimpl->layer_list.end(), dst);
  pimpl->layer_list.insert(it, layer);
  layer->registerManager(this);

  // !!! このときに、SurfaceList の更新も行う
}


void LayerManager::remove(Layer* layer) {

  pImpl::LayerList::iterator it = find(pimpl->layer_list.begin(),
                                       pimpl->layer_list.end(), layer);
  if (it != pimpl->layer_list.end()) {
    pimpl->layer_list.erase(it);
  }
}


void LayerManager::createComponentList(ComponentList& components) {

  components.clear();

  for (pImpl::LayerList::iterator it = pimpl->layer_list.begin();
       it != pimpl->layer_list.end(); ++it) {
    (*it)->addComponentList(components);
  }
}


void LayerManager::createSurfaceList(SurfaceList& surfaces,
                                     const ComponentList& components,
                                     size_t ticks) {

  // この時点では、前回に再描画した時のサーフェスリストが保持されている

  //fprintf(stderr, "createSurfaceList: %d\n", surfaces.size());

  // 変更されたサーフェスはここで取り除き、改めて再描画を行う
  // !!! 改めての再描画は、この次の redraw() で行われるはず
  SurfaceList changed_surfaces;
  for (SurfaceList::iterator it = surfaces.begin();
       it != surfaces.end(); ++it) {

    // !!! このあたり、位置の変化を検出してないのは、問題ないのかな？

    // !!! 変な仕組みだな...
    if ((*it)->surface->isChanged(ticks)) {
      //fprintf(stderr, "is_changed !\n");
      // redraw() で描画されるように、フラグをセットしておく
      (*it)->surface->forceSetChanged();
      changed_surfaces.push_back(*it);
    }
  }
  //fprintf(stderr, "changed_surfaces: %d\n", changed_surfaces.size());
  removeSurface(changed_surfaces);

  // 削除領域の更新
  // !!! 前回の描画から変更された箇所を、背面にあるサーフェスで描画する

  // !!! さすがに、このあたりは専用の関数名にしてしまいましょう

  // !!! とりあえず削除の範囲で一度描画してしまう
  // !!! 本来は、再描画は最後にまとめて行えばよいはず

  for (SurfaceList::iterator it = surfaces.begin();
       it != surfaces.end(); ++it) {
    pImpl::Rects& redraw_rects = (*it)->redraw_rects;
#if 0
    if (! redraw_rects.empty()) {
      fprintf(stderr, "redraw_rects: %d\n", redraw_rects.size());
    }
#endif
    for (pImpl::Rects::iterator draw_it = redraw_rects.begin();
         draw_it != redraw_rects.end(); ++draw_it) {
      SDL_Rect draw_area;
      set_SdlRect(&draw_area, draw_it->x - (*it)->position.x,
                  draw_it->y - (*it)->position.y, draw_it->w, draw_it->h);
      //fprintf(stderr, "redraw_rect: %d, %d, %d, %d\n", draw_area.x, draw_area.y, draw_area.w, draw_area.h);
#if 0
      SDL_FillRect(pimpl->scr, &draw_area, pimpl->back_color);
#else
      (*it)->surface->draw(pimpl->remove_update_rects, &*draw_it, &draw_area);
#endif
    }
    redraw_rects.clear();
  }
  // !!! 上記を関数化する


  surfaces.clear();
  SDL_Rect area;
  set_SdlRect(&area, 0, 0,
              static_cast<int>(pimpl->width -1),
              static_cast<int>(pimpl->height -1));

  for (ComponentList::const_iterator it = components.begin();
       it != components.end(); ++it) {
    (*it)->addSurfaceList(surfaces, &area, ticks);
  }
}


void LayerManager::removeSurface(SurfaceList& remove_list) {

  // 手前から削除サーフェスまで、再描画領域を評価する

  // !!! とりあえず、全画面に対して評価を行う
  // !!! 画面幅は、本来はちゃんと取得すべき
  SDL_Rect all_area;
  set_SdlRect(&all_area, 0, 0, 640, 480);
  pImpl::Rects redraw_areas;
  redraw_areas.push_back(all_area);

  // 見える領域
  for (pImpl::Rects::iterator redraw_it = redraw_areas.begin();
       redraw_it != redraw_areas.end(); ++redraw_it) {

    // 削除サーフェス
    // !!! ここで、削除するサーフェス間で、重なり領域を評価しておく方がよさそう
    for (SurfaceList::iterator remove_it = remove_list.begin();
         remove_it != remove_list.end(); ++remove_it) {

      //fprintf(stderr, "%d, %d, %d, %d\n", (*remove_it)->position.x, (*remove_it)->position.y, (*remove_it)->area.w, (*remove_it)->area.h);

      pImpl::Rects remove_area;
      add_SdlRect(remove_area,
                  (*remove_it)->position.x, (*remove_it)->position.y,
                  (*remove_it)->area.w, (*remove_it)->area.h);

      pImpl::Rects common_area; // 使わない
      pImpl::Rects unique_area; // 削除するサーフェスの見えている部分
      bool front_surface = true;

      // 管理しているサーフェス
      for (SurfaceList::iterator surface_it = pimpl->surface_list.begin();
           surface_it != pimpl->surface_list.end();) {
        //fprintf(stderr, "cmp: %p\n", *surface_it);
        SDL_Rect surface_area;
        set_SdlRect(&surface_area,
                    (*surface_it)->position.x, (*surface_it)->position.y,
                    (*surface_it)->area.w, (*surface_it)->area.h);

        if (front_surface) {
          // 前面サーフェスとの評価
          // 見えている領域を remove_area に登録していく

          if (*surface_it == *remove_it) {
            //fprintf(stderr, "match equal.\n");

            // 自サーフェスまで評価したら、次は背面の評価を行う
            front_surface = false;

            // 自サーフェスは削除されるため、登録されている再描画領域は次に渡す
            // !!! この処理、意味あるのかな？
            for (pImpl::Rects::iterator it =
                   (*surface_it)->redraw_rects.begin();
                 it != (*surface_it)->redraw_rects.end(); ++it) {
              remove_area.push_back(*it);
            }
            (*surface_it)->redraw_rects.clear();

            // 自サーフェスの登録を削除
            surface_it = pimpl->surface_list.erase(surface_it);
            // !!! この -- は、変じゃないか？
            //--surface_it;
            // !!! surface_it が front() のときに問題がありそう
            // !!! まぁ、ループで ++ してるから実質的な問題はないと思うけどね...

          } else {
            // 削除するサーフェスの見えている部分を remove_area に登録する
            unique_area.clear();
            common_area.clear();
            for (pImpl::Rects::iterator unique_it = remove_area.begin();
                 unique_it != remove_area.end(); ++unique_it) {

              evaluateArea(common_area, unique_area, *unique_it, surface_area);

              // !!! 以降、関数化してくくる。いずれ
              // 透過サーフェスのときは、common を以降のサーフェスに渡す
              if ((*surface_it)->is_transparent) {
                unique_area.insert(unique_area.end(),
                                   common_area.begin(), common_area.end());
              }
            }
            remove_area = unique_area;
            ++surface_it;
          }
        } else {
          // !!! この else は、２つの関数にしてしまいたいかも

          // 背面サーフェスとの評価
          // remove_area の領域を、背面にあるサーフェスに再描画させる

          common_area.clear();
          unique_area.clear();
          for (pImpl::Rects::iterator it = remove_area.begin();
               it != remove_area.end(); ++it) {

            // 重なり判定を行い、common をそのサーフェス
            // unique を次のサーフェスへ
            evaluateArea(common_area, unique_area, *it, surface_area);
            //fprintf(stderr, "(%d, %d, %d, %d) (%d, %d, %d, %d) ", it->x, it->y, it->w, it->h, surface_area.x, surface_area.y, surface_area.w, surface_area.h);
            //fprintf(stderr, "common: %d, unique: %d\n", common_area.size(), unique_area.size());
            pImpl::Rects::iterator last_it = (*surface_it)->redraw_rects.end();
            (*surface_it)->redraw_rects.insert(last_it, common_area.begin(),
                                               common_area.end());
            common_area.clear();

            // 透過サーフェスのときは、common を以降のサーフェスに渡す
            if ((*surface_it)->is_transparent) {
              unique_area.insert(unique_area.end(),
                                 common_area.begin(), common_area.end());
            }
          }
          remove_area = unique_area;
          ++surface_it;
        }
      }
      // remove_area を背景色で塗りつぶす領域として、再描画領域に登録する
      for (pImpl::Rects::iterator it = remove_area.begin();
           it != remove_area.end(); ++it) {
        pimpl->back_rects.push_back(*it);
      }
    }
  }
}
