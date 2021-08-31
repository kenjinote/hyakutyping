/*!
  \file
  \brief メニューの更新補助

  \author Satofumi KAMIMURA

  $Id$
*/

#include "UpdateUtils.h"
#include "CommonResources.h"
#include "GuiManager.h"
#include "InputHandler.h"
#include "InputEvent.h"

using namespace beego;


struct UpdateUtils::pImpl {
  GuiManager* gui_;
  InputHandler* input_;
  InputEvent input_event_;

  pImpl(CommonResources* common) : gui_(common->gui_), input_(common->input_)
  {
  }
};


UpdateUtils::UpdateUtils(CommonResources* common) : pimpl(new pImpl(common)) {
}


UpdateUtils::~UpdateUtils(void) {
}


bool UpdateUtils::update(void) {

  // イベントの更新
  pimpl->gui_->update();
  pimpl->input_->update_all();

  // 終了イベントの取得
  pimpl->input_->updateInputEvent(pimpl->input_event_);
  bool quit =
    pimpl->input_->haveQuitEvent() |
    pimpl->input_->isReleased(SDLK_ESCAPE) |
    pimpl->input_event_.right_clicked;

  return quit;
}


bool UpdateUtils::haveQuitEvent(void) {
  return pimpl->input_->haveQuitEvent();
}
