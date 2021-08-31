#ifndef USER_MENU_H
#define USER_MENU_H

/*!
  \file
  \brief ユーザメニュー

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ユーザ管理メニュー
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
    \brief 初期メニューの開始
  */
  void run(void);
};

#endif /* !USER_MENU_H */
