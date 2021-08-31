#ifndef PRACTICE_MENU_H
#define PRACTICE_MENU_H

/*!
  \file
  \brief �^�C�s���O���K�̃��j���[

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �^�C�s���O���K�̃��j���[�Ǘ�
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
    \brief ���K���j���[�̊J�n
  */
  void run(void);
};

#endif /* !PRACTICE_MENU_H */
