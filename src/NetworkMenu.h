#ifndef NETWORK_MENU_H
#define NETWORK_MENU_H

/*!
  \file
  \brief �l�b�g���[�N�̃��j���[���

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �ŏ��̃��j���[�Ǘ�
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
    \brief �l�b�g���[�N���j���[�̊J�n
  */
  void run(void);
};

#endif /* !NETWORK_MENU_H */
