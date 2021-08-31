#ifndef REMOVE_USER_H
#define REMOVE_USER_H

/*!
  \file
  \brief ユーザ削除

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief ユーザ削除
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
    \brief 起動
  */
  void run(void);
};

#endif /* !REMOVE_USER_H */
