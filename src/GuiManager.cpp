/*!
  \file
  \brief GUI 管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "GuiManager.h"
#include "SdlVideo.h"
#include "ComponentInterface.h"
#include "LayerManager.h"
#include "SurfaceInterface.h"
#include "GetTicks.h"
#include "InputHandler.h"
#include "InputEvent.h"

using namespace beego;


struct GuiManager::pImpl {
  enum {
    DefaultFpsWait = 15,
  };
  SDL::SdlVideo video_;
  SurfaceList surface_list_;
  LayerManager layer_manager_;
  SDL_Surface* scr_;
  std::vector<SDL_Rect> update_rects_;
  ComponentList component_list_;
  InputEvent input_event_;
  InputHandler input_;
  std::vector<SDL_Rect> applied_rects_;

  pImpl(void) : layer_manager_(surface_list_), scr_(SDL_GetVideoSurface()) {
  }

  ~pImpl(void) {
    // !!! Layer を remove すべき
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }

  void update(void) {

    // 入力情報の更新
    input_.updateInputEvent(input_event_);

    // スクリーンの切り替え
    // !!! 適当だなぁ...
#if 0
    // !!! 再描画が行われなかったので、コメントアウト
    if (input_.haveToggleScreenEvent()) {
      video_.setFullscreenMode(! video_.isFullscreen());
      video_.hide();
      video_.show();
      scr_ = SDL_GetVideoSurface();
    }
#endif

    // !!! 描画の平均時間を管理しておき、
    // !!! 再描画が DefaultFpsWait より早く終わりそうなときは、再描画しない
    // !!! という風に変更する。いずれ
    redraw();
  }

  void redraw(void) {
    // !!! 必要に応じて、システムに登録した ticks を使うように変更する
    size_t ticks = GetTicks();

    // サーフェスリストの再構築
    layer_manager_.createComponentList(component_list_);
    layer_manager_.createSurfaceList(surface_list_, component_list_, ticks);

    // 入力イベントの反映
    applied_rects_.clear();
    layer_manager_.applyInput(component_list_, input_event_, applied_rects_);

    // 再描画
    update_rects_.clear();
    layer_manager_.redraw(update_rects_, surface_list_);

    if (scr_ && (! update_rects_.empty())) {
#if 0
      int n = update_rects_.size();
      for (int i = 0; i < n; ++i) {
        fprintf(stderr, "[%d, %d, %d, %d] ", update_rects_[i].x, update_rects_[i].y, update_rects_[i].w, update_rects_[i].h);
      }
      fprintf(stderr, "\n");
#endif

      SDL_UpdateRects(scr_, static_cast<int>(update_rects_.size()),
                      &update_rects_[0]);
    }
  }
};


GuiManager::GuiManager(void) : pimpl(pImpl::getObject()) {
}


GuiManager::~GuiManager(void) {
}


void GuiManager::push_front(Layer* layer) {
  pimpl->layer_manager_.push_front(layer);
}


void GuiManager::push_back(Layer* layer) {
  pimpl->layer_manager_.push_back(layer);
}


void GuiManager::insert(const Layer* dst, Layer* layer) {
  pimpl->layer_manager_.insert(dst, layer);
}


void GuiManager::remove(Layer* layer) {
  pimpl->layer_manager_.remove(layer);
}


void GuiManager::update(void) {
  pimpl->update();
}


void GuiManager::redraw(void) {
  pimpl->redraw();
}
