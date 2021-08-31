#ifndef CONNECTION_INTERFACE_H
#define CONNECTION_INTERFACE_H

/*!
  \file
  \brief 通信デバイスのインターフェース定義

  \author Satofumi KAMIMURA

  $Id$
*/


namespace beego {
  /*!
    \brief 通信インターフェース
  */
  class ConnectionInterface {
  public:
    virtual ~ConnectionInterface(void) {}

    /*!
      \brief 内部状態を返す

      \return 内部状態を示す文字列
    */
    virtual const char* what(void) = 0;

    /*!
      \brief デバイスへの接続

      \param device [i] 接続デバイス名
      \param baudrate [i] 接続ボーレート

      \retval true 成功
      \retval false 失敗
    */
    virtual bool connect(const char* device, long baudrate) = 0;

    /*!
      \brief 切断
    */
    virtual void disconnect(void) = 0;

    /*!
      \brief 接続状態を返す

      \retval true 接続中
      \retval false 切断中
    */
    virtual bool isConnected(void) = 0;

    /*!
      \brief ボーレートの変更

      \param baudrate [i] ボーレート

      \retval true 正常に変更
      \retval false 変更に失敗
    */
    virtual bool changeBaudrate(long baudrate) = 0;

    /*!
      \brief 送信

      \param data [i] 送信データ
      \param size [i] 送信バイト数

      \return 送信したバイト数
    */
    virtual int send(const char* data, int size) = 0;

    /*!
      \brief 受信

      \param data [o] 受信用バッファ
      \param size [i] 受信バッファの最大サイズ
      \param timeout [i] タイムアウト時間 [msec]
    */
    virtual int recv(char* data, int size, int timeout) = 0;

    /*!
      \brief 受信済みデータのバイト数を返す

      \param timeout [i] タイムアウト時間 [msec]

      \return 受信済みデータのバイト数
    */
    virtual int size(int timeout = 0) = 0;

    /*!
      \brief 送受信データのクリア

      送信バッファ、受信済みデータをクリアする
    */
    virtual void clear(void) = 0;

    /*!
      \brief 受信データの破棄

      受信データを読み捨てる

      \param total_timeout [i] 受信トータルのタイムアウト時間 [msec]
    */
    virtual void skip(int total_timeout) = 0;

    virtual void flush(void) = 0;
  };
};

#endif /* !CONNECTION_INTERFACE_H */
