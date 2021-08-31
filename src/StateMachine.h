#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/*!
  \file
  \brief ステートの実行テンプレート

  \author Satofumi KAMIMURA

  $Id$

  「実例で学ぶゲーム AI プログラム」(O'REILLY) より
*/

#include "State.h"
#include <stdio.h>


namespace beego {
  template <class entity_type>
  class StateMachine {
    entity_type* owner;
    State<entity_type>* current_state;
    State<entity_type>* previous_state;
    State<entity_type>* global_state;

  public:
    StateMachine(entity_type* owner_obj)
      : owner(owner_obj), current_state(NULL), previous_state(NULL),
        global_state(NULL) {
    }

    void setCurrentState(State<entity_type>* state) {
      current_state = state;
    }

    void setGlobalState(State<entity_type>* state) {
      global_state = state;
    }

    void setPreviousState(State<entity_type>* state) {
      previous_state = state;
    }

    void update(void) const {
      if (global_state) {
        global_state->execute(owner);
      }

      if (current_state) {
        current_state->execute(owner);
      }
    }

    void changeState(State<entity_type>* new_state) {
      if (new_state == NULL) {
        return;
      }

      previous_state = current_state;
      current_state->exit(owner);
      current_state = new_state;
      current_state->enter(owner);
    }

    void revertToPreviousState(void) {
      changeState(previous_state);
    }

    State<entity_type>* currentState(void) const {
      return current_state;
    }

    State<entity_type>* globalState(void) const {
      return global_state;
    }

    State<entity_type>* previousState(void) const {
      return previous_state;
    }

    //bool isInstance(const State<entity_type>& st) const;
  };
};

#endif /*! STATE_MACHINE_H */
