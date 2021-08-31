/*!
  \file
  \brief GUI �Ǘ�

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
    // !!! Layer �� remove ���ׂ�
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }

  void update(void) {

    // ���͏��̍X�V
    input_.updateInputEvent(input_event_);

    // �X�N���[���̐؂�ւ�
    // !!! �K�����Ȃ�...
#if 0
    // !!! �ĕ`�悪�s���Ȃ������̂ŁA�R�����g�A�E�g
    if (input_.haveToggleScreenEvent()) {
      video_.setFullscreenMode(! video_.isFullscreen());
      video_.hide();
      video_.show();
      scr_ = SDL_GetVideoSurface();
    }
#endif

    // !!! �`��̕��ώ��Ԃ��Ǘ����Ă����A
    // !!! �ĕ`�悪 DefaultFpsWait ��葁���I��肻���ȂƂ��́A�ĕ`�悵�Ȃ�
    // !!! �Ƃ������ɕύX����B������
    redraw();
  }

  void redraw(void) {
    // !!! �K�v�ɉ����āA�V�X�e���ɓo�^���� ticks ���g���悤�ɕύX����
    size_t ticks = GetTicks();

    // �T�[�t�F�X���X�g�̍č\�z
    layer_manager_.createComponentList(component_list_);
    layer_manager_.createSurfaceList(surface_list_, component_list_, ticks);

    // ���̓C�x���g�̔��f
    applied_rects_.clear();
    layer_manager_.applyInput(component_list_, input_event_, applied_rects_);

    // �ĕ`��
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
