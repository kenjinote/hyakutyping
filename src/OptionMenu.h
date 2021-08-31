#ifndef OPTION_MENU_H
#define OPTION_MENU_H

/*!
  \file
  \brief オプションメニュー

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief オプション項目の管理
*/
class OptionMenu {
  OptionMenu(const OptionMenu& rhs);
  OptionMenu& operator = (const OptionMenu& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  OptionMenu(void);
  ~OptionMenu(void);

  /*!
    \brief オプションメニューの開始
  */
  void run(void);
};

#endif /* !OPTION_MENU_H */
