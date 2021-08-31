#ifndef REMOVE_USER_H
#define REMOVE_USER_H

/*!
  \file
  \brief ���[�U�폜

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ���[�U�폜
*/
class RemoveUser {
  RemoveUser(const RemoveUser& rhs);
  RemoveUser& operator = (const RemoveUser& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  RemoveUser(void);
  ~RemoveUser(void);

  /*!
    \brief �N��
  */
  void run(void);
};

#endif /* !REMOVE_USER_H */
