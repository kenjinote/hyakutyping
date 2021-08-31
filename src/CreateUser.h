#ifndef CREATE_USER_H
#define CREATE_USER_H

/*!
  \file
  \brief ���[�U�쐬

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ���[�U�쐬
*/
class CreateUser {
  CreateUser(const CreateUser& rhs);
  CreateUser& operator = (const CreateUser& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  CreateUser(void);
  ~CreateUser(void);

  /*!
    \brief �N��
  */
  void run(void);
};

#endif /* !CREATE_USER_H */
