#ifndef NETWORK_MENU_H
#define NETWORK_MENU_H

/*!
  \file
  \brief ネットワークのメニュー画面

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief 最初のメニュー管理
*/
class NetworkMenu {
  NetworkMenu(const NetworkMenu& rhs);
  NetworkMenu& operator = (const NetworkMenu& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  NetworkMenu(void);
  ~NetworkMenu(void);

  /*!
    \brief ネットワークメニューの開始
  */
  void run(void);
};

#endif /* !NETWORK_MENU_H */
