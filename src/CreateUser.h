#ifndef CREATE_USER_H
#define CREATE_USER_H

/*!
  \file
  \brief ユーザ作成

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ユーザ作成
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
    \brief 起動
  */
  void run(void);
};

#endif /* !CREATE_USER_H */
