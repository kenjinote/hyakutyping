/*!
  \file
  \brief 履歴の閲覧

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ViewHistory.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "GraphDrawer.h"
#include "UpdateUtils.h"
#include "Layer.h"
#include "InputEvent.h"
#include "InputHandler.h"
#include "InputReceiveComponent.h"
#include "BaseEntity.h"
#include "StateMachine.h"
#include "Delay.h"

using namespace beego;


struct ViewHistory::pImpl {

  class Schedule : public BaseEntity {
    bool is_terminated_;
    StateMachine<Schedule>* state_machine_;

  public:
    Schedule(int id, CommonResources* common)
      : BaseEntity(id), is_terminated_(false) {
      state_machine_ = new StateMachine<Schedule>(this);
      state_machine_->setCurrentState(FirstState::getObject(common));
    }

    ~Schedule(void) {
      delete state_machine_;
    }

    void update(void) {
      state_machine_->update();
    }

    void changeState(State<Schedule>* new_state) {
      state_machine_->changeState(new_state);
    }

    void setTerminate(void) {
      is_terminated_ = true;
    }

    bool isTerminated(void) {
      return is_terminated_;
    }
  };

  class FirstState : public State<Schedule> {
    CommonResources* common_;
    InputEvent input_event_;

    FirstState(CommonResources* common) : common_(common) {
    }

  public:
    static FirstState* getObject(CommonResources* common) {
      static FirstState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      type->setTerminate();
    }

    void execute(Schedule* type) {
      common_->input_receiver_->updateInputEvent(input_event_);
      if (InputEvent::isPressed(input_event_, SDLK_RETURN) ||
          input_event_.left_released) {
        type->changeState(ShimonokuViewState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }
  };

  class ShimonokuViewState : public State<Schedule> {
    CommonResources* common_;
    InputEvent input_event_;
    GraphDrawer graph_;

    ShimonokuViewState(CommonResources* common)
      : common_(common), graph_(ShimonokuTyping) {
    }

  public:
    static ShimonokuViewState* getObject(CommonResources* common) {
      static ShimonokuViewState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      graph_.drawGraph();
      placeComponents();
    }

    void execute(Schedule* type) {

      common_->input_receiver_->updateInputEvent(input_event_);
      if (InputEvent::isReleased(input_event_, SDLK_RETURN) ||
          input_event_.left_released) {
        type->changeState(KimarijiViewState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {

      graph_.placeComponents();
      common_->input_receiver_->clear();
    }

    void removeComponents(void) {

      graph_.removeComponents();
      common_->input_receiver_->clear();
    }
  };

  class KimarijiViewState : public State<Schedule> {
    CommonResources* common_;
    InputEvent input_event_;
    GraphDrawer graph_;

    KimarijiViewState(CommonResources* common)
      : common_(common), graph_(KimarijiTyping) {
    }

  public:
    static KimarijiViewState* getObject(CommonResources* common) {
      static KimarijiViewState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // 効果音を鳴らす
      common_->playEffect(Decide);

      graph_.drawGraph();
      placeComponents();
    }

    void execute(Schedule* type) {

      common_->input_receiver_->updateInputEvent(input_event_);
      if (InputEvent::isReleased(input_event_, SDLK_RETURN) ||
          input_event_.left_released) {
        type->changeState(FirstState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {

      graph_.placeComponents();
      common_->input_receiver_->clear();
    }

    void removeComponents(void) {

      graph_.removeComponents();
      common_->input_receiver_->clear();
    }
  };

  CommonResources* common_;
  Schedule scheduler_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      scheduler_(0, common_) {
  }

  void placeComponents(void) {
  }

  void removeComponents(void) {
    ShimonokuViewState::getObject(common_)->removeComponents();
    KimarijiViewState::getObject(common_)->removeComponents();
  }
};


ViewHistory::ViewHistory(void) : pimpl(new pImpl) {
}


ViewHistory::~ViewHistory(void) {
}


void ViewHistory::run(void) {

  UpdateUtils updater(pimpl->common_);
  bool quit = false;
  while (quit == false) {

    // 状態の更新
    pimpl->scheduler_.update();

    // 終了判定
    quit |= updater.update() | pimpl->scheduler_.isTerminated();
    delay(1);
  }

  // 各コンポーネントの削除
  pimpl->removeComponents();
}
