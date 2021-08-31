#ifndef FIRST_MENU_H
#define FIRST_MENU_H

/*!
  \file
  \brief �N����̃��j���[���

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �ŏ��̃��j���[�Ǘ�
*/
class FirstMenu {
  FirstMenu(const FirstMenu& rhs);
  FirstMenu& operator = (const FirstMenu& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  FirstMenu(void);
  ~FirstMenu(void);

  /*!
    \brief �������j���[�̊J�n
  */
  void run(void);
};

#endif /* !FIRST_MENU_H */
