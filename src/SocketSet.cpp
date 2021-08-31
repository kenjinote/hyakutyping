/*!
  \file
  \brief 通信ソケット管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SocketSet.h"
#include "TcpipCtrl.h"

using namespace beego;


/*!
  \brief SocketSet の内部クラス
*/
struct SocketSet::pImpl {
  size_t max_num;
  size_t hold_num;
  SDLNet_SocketSet socket_set;

#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  pImpl(size_t size)
    : max_num(size), hold_num(0),
      socket_set(SDLNet_AllocSocketSet(static_cast<int>(max_num))) {
  }
#else
  pImpl(size_t size) : max_num(size), hold_num(0), socket_set(NULL) {
  }
#endif

  ~pImpl(void) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    SDLNet_FreeSocketSet(socket_set);
#endif
  }
};


SocketSet::SocketSet(size_t size) : pimpl(new pImpl(size)) {
}


SocketSet::~SocketSet(void) {
}


bool SocketSet::add(TCPsocket socket) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  if (pimpl->hold_num < pimpl->max_num) {
    if (SDLNet_TCP_AddSocket(pimpl->socket_set, socket) >= 0) {
      ++(pimpl->hold_num);
      return true;
    }
  }
  return false;
#else
  return false;
#endif
}


void SocketSet::del(TCPsocket socket) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  if ((pimpl->hold_num > 0) &&
      (SDLNet_TCP_DelSocket(pimpl->socket_set, socket) >= 0)) {
    --(pimpl->hold_num);
  }
#endif
}


size_t SocketSet::check(int timeout) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  return SDLNet_CheckSockets(pimpl->socket_set, timeout);
#else
  return 0;
#endif
}
