#ifndef PRACTICE_MENU_H
#define PRACTICE_MENU_H

/*!
  \file
  \brief タイピング練習のメニュー

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief タイピング練習のメニュー管理
*/
class PracticeMenu {
  PracticeMenu(const PracticeMenu& rhs);
  PracticeMenu& operator = (const PracticeMenu& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  PracticeMenu(void);
  ~PracticeMenu(void);

  /*!
    \brief 練習メニューの開始
  */
  void run(void);
};

#endif /* !PRACTICE_MENU_H */
