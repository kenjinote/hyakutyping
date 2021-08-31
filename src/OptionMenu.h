#ifndef OPTION_MENU_H
#define OPTION_MENU_H

/*!
  \file
  \brief �I�v�V�������j���[

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �I�v�V�������ڂ̊Ǘ�
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
    \brief �I�v�V�������j���[�̊J�n
  */
  void run(void);
};

#endif /* !OPTION_MENU_H */
