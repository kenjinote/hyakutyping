#ifndef TCPIP_CTRL_H
#define TCPIP_CTRL_H

/*!
  \file
  \brief TCP/IP ê⁄ë±

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ConnectionInterface.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
#include <SDL_net.h>
#else
namespace beego {
  class TCPsocket {
  public:
    TCPsocket(void *dummy) {
    }
  };

  class SDLNet_SocketSet {
  public:
    SDLNet_SocketSet(void *dummy) {
    }
  };

  class IPaddress {
  };
};
#endif
#include <memory>

namespace beego {

  class SocketSet;


  /*!
    \brief TCP/IP ê⁄ë±ÉnÉìÉhÉâ
  */
  class TcpipCtrl : public ConnectionInterface {
    TcpipCtrl(const TcpipCtrl& rhs);
    TcpipCtrl& operator = (const TcpipCtrl& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum { NoTimeout = -1 };
    TcpipCtrl(void);
    explicit TcpipCtrl(SocketSet* socketSet, TCPsocket socket = NULL);
    ~TcpipCtrl(void);

    const char* what(void);
    bool connect(const char* host, long port);
    void disconnect(void);
    bool isConnected(void);
    bool changeBaudrate(long baudrate);
    int send(const char* data, int size);
    int recv(char* data, int size, int timeout);
    int size(int timeout = 0);
    void clear(void);
    void skip(int total_timeout);
    void flush(void);

    TCPsocket getNetSocket(void);
  };
};

#endif /* !TCPIP_CTRL_H */
