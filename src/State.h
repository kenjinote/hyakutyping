#ifndef STATE_H
#define STATE_H

/*!
  \file
  \brief ステートの基底テンプレート

  \author Satofumi KAMIMURA

  $Id$

  「実例で学ぶゲーム AI プログラム」(O'REILLY) より
*/

namespace beego {
  template <class entity_type>
  class State {
  public:
    virtual ~State(void) {}

    virtual void enter(entity_type* type) = 0;
    virtual void execute(entity_type* type) = 0;
    virtual void exit(entity_type* type) = 0;
  };
};

#endif /*! STATE_H */
