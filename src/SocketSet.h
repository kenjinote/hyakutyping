#ifndef SOCKET_SET_H
#define SOCKET_SET_H

/*!
  \file
  \brief 通信ソケット管理

  \author Satofumi KAMIMURA

  $Id$
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
#include <SDL_net.h>
#else
class TCPsocket;
#endif
#include <memory>

namespace beego {

  class TcpipCtrl;


  /*!
    \brief 通信ソケット管理
  */
  class SocketSet {
    SocketSet(const SocketSet& rhs);
    SocketSet& operator = (const SocketSet& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    /*!
      \brief コンストラクタ

      \param size [i] 初期状態でのソケット許容数
    */
    explicit SocketSet(size_t size = 1);
    ~SocketSet(void);

    /*!
      \brief ソケットの登録

      \param socket [i] 登録するソケット

      \retval true 登録に成功
      \retval false 登録に失敗
    */
    bool add(TCPsocket socket);

    /*!
      \brief ソケットの削除

      \param socket [i] 削除するソケット
    */
    void del(TCPsocket socket);

    /*!
      \brief 受信データがあるかの確認

      \param timeout [i] タイムアウト時間 [msec]

      \return 受信データのあるソケット数
    */
    size_t check(int timeout);
  };
};

#endif /* !SOCKET_SET_H */
