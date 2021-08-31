#ifndef FIRST_MENU_H
#define FIRST_MENU_H

/*!
  \file
  \brief 起動後のメニュー画面

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief 最初のメニュー管理
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
    \brief 初期メニューの開始
  */
  void run(void);
};

#endif /* !FIRST_MENU_H */
