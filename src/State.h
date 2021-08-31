#ifndef STATE_H
#define STATE_H

/*!
  \file
  \brief �X�e�[�g�̊��e���v���[�g

  \author Satofumi KAMIMURA

  $Id$

  �u����Ŋw�ԃQ�[�� AI �v���O�����v(O'REILLY) ���
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
