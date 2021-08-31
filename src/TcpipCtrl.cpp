/*!
  \file
  \brief TCP/IP 接続

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TcpipCtrl.h"
#include "SocketSet.h"
#include "RingBuffer.h"
#include "NetInit.h"
#include <string>

using namespace beego;


/*!
 \brief TcpipCtrl の内部クラス
*/
struct TcpipCtrl::pImpl : private NetInit {
  std::string error_message;
  TCPsocket socket;
  SocketSet* socket_set;
  SDLNet_SocketSet own_set;
  RingBuffer<char> ring_buffer;
  bool have_data;

  pImpl(SocketSet* socketSet, TCPsocket tcp_socket)
    : error_message("not connected."), socket(tcp_socket),
      socket_set(socketSet), own_set(NULL), have_data(false) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    if (socket_set == NULL) {
      own_set = SDLNet_AllocSocketSet(1);
      if (socket != NULL) {
        addSocket(socket);
      }
    } else {
      socket_set->add(socket);
    }
#endif
  }

  ~pImpl(void) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    disconnect();
    if (own_set) {
      SDLNet_FreeSocketSet(own_set);
    }
#endif
  }

  void disconnect(void) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    if (socket) {
      if (socket_set) {
        socket_set->del(socket);
      } else {
        SDLNet_TCP_DelSocket(own_set, socket);
      }

      //fprintf(stderr, " close %p.\n", socket);
      SDLNet_TCP_Close(socket);
      socket = NULL;
    }
    error_message = "disconnected.";
#endif
  }

  void addSocket(TCPsocket tcp_socket) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    SDLNet_TCP_AddSocket(own_set, tcp_socket);
#endif
  }

  int update(int timeout, int size = 0) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    enum { BufferSize = 1024 };
    int require_size = (size > 0) ? size : BufferSize;
    int filled = 0;
    while (filled < require_size) {
      if (socket_set == NULL) {
        if (SDLNet_CheckSockets(own_set, (size == 0) ? 0 : timeout) <= 0) {
          //if (SDLNet_CheckSockets(own_set, timeout) <= 0) {
          break;
        }
      }
      if (SDLNet_SocketReady(socket) <= 0) {
        break;
      }
      char data[BufferSize];
      int read_n = require_size - filled;
      int n = SDLNet_TCP_Recv(socket, data,
                              (read_n > BufferSize) ? BufferSize : read_n);
      if (n <= 0) {
        disconnect();
        return -1;
      }
      ring_buffer.put(data, n);
      have_data = true;
      filled += n;

      if (socket_set != NULL) {
        // SocketSet を使ったチェックの関係で、読み込みを行うのは１回のみ
        break;
      }
    }
    return 0;
#else
    return -1;
#endif
  }
};


TcpipCtrl::TcpipCtrl(void) : pimpl(new pImpl(NULL, NULL)) {
}


TcpipCtrl::TcpipCtrl(SocketSet* socketSet, TCPsocket tcp_socket)
  : pimpl(new pImpl(socketSet, tcp_socket)) {
}


TcpipCtrl::~TcpipCtrl(void) {
}


const char* TcpipCtrl::what(void) {
  return pimpl->error_message.c_str();
}


bool TcpipCtrl::connect(const char* host, long port) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  IPaddress ip;
  int ret = SDLNet_ResolveHost(&ip, host, static_cast<short>(port));
  if (ret < 0) {
    // !!! エラー出力
    return false;
  }

  TCPsocket socket = SDLNet_TCP_Open(&ip);
  if (socket == NULL) {
    char strnum[13];
    sprintf(strnum, "%ld", port);
    pimpl->error_message =
      std::string(SDLNet_GetError()) + " (" + host + ", " + strnum + ")";
    return false;
  }
  //fprintf(stderr, " open %p\n", socket);
  disconnect();
  std::swap(socket, pimpl->socket);

  // ソケットを登録
  if (pimpl->socket_set) {
    pimpl->socket_set->add(pimpl->socket);
  } else {
    pimpl->addSocket(pimpl->socket);
  }
  pimpl->error_message = "connected.";
  return true;
#else
  return false;
#endif
}


void TcpipCtrl::disconnect(void) {
  pimpl->disconnect();
}


/*!
  \todo 受信をチェックして切断検出の必要があるかを検討する。実装済み？
*/
bool TcpipCtrl::isConnected(void) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  return (pimpl->socket == NULL) ? false : true;
#else
  return false;
#endif
}


bool TcpipCtrl::changeBaudrate(long baudrate) {
  // 実装しない
  return true;
}


int TcpipCtrl::send(const char* data, int size) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  if (! isConnected()) {
    return -1;
  }
  return SDLNet_TCP_Send(pimpl->socket, const_cast<char*>(data), size);
#else
  return -1;
#endif
}


int TcpipCtrl::recv(char* data, int size, int timeout) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
  if (((isConnected() == false) || (size <= 0))
      && (pimpl->have_data == false)) {
    return 0;
  }
  // 受信データの更新
  if ((pimpl->update(timeout) < 0) && (pimpl->have_data == false)) {
    // 切断を検出
    return -1;
  }

  // 要求分だけ受信データがあれば、返す
  int filled = pimpl->ring_buffer.size();
  if (filled >= size) {
    pimpl->ring_buffer.get(data, size);
    pimpl->have_data = ! pimpl->ring_buffer.empty();
    return size;
  }

  // 受信できたデータを返す
  int left = size - filled;
  if ((pimpl->own_set != NULL) && (pimpl->update(timeout, left) < 0)
      && (pimpl->have_data == false)) {
    // 切断を検出
    return -1;
  }

  // size() を呼ぶと update(0) が呼ばれるため
  filled = pimpl->ring_buffer.size();
  pimpl->ring_buffer.get(data, filled);
  pimpl->have_data = ! pimpl->ring_buffer.empty();

  return filled;
#else
  return -1;
#endif
}


int TcpipCtrl::size(int timeout) {
  if ((pimpl->update(timeout) < 0) && (pimpl->have_data == false)) {
    return -1;
  }
  return pimpl->ring_buffer.size();
}


void TcpipCtrl::clear(void) {
  pimpl->ring_buffer.clear();
}


TCPsocket TcpipCtrl::getNetSocket(void) {
  return pimpl->socket;
}


void TcpipCtrl::skip(int total_timeout) {
  clear();

  char buffer[BUFSIZ];
  int total = 0;
  int n;

  enum { EachTimeout = 10 };
  do {
    n = recv(buffer, BUFSIZ, EachTimeout);
    total += EachTimeout;
  } while ((n > 0) && ((total_timeout > 0) && (total < total_timeout)));
}


void TcpipCtrl::flush(void) {
  // !!!
}
