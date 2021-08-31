#ifndef USER_MENU_H
#define USER_MENU_H

/*!
  \file
  \brief ���[�U���j���[

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ���[�U�Ǘ����j���[
*/
class UserMenu {
  UserMenu(const UserMenu& rhs);
  UserMenu& operator = (const UserMenu& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  UserMenu(void);
  ~UserMenu(void);

  /*!
    \brief �������j���[�̊J�n
  */
  void run(void);
};

#endif /* !USER_MENU_H */
