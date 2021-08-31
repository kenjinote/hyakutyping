#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

/*!
  \file
  \brief ログ管理クラス

  \author Satofumi KAMIMURA

  $Id$

  \todo ID 番号で書き込みファイルを指定できるように変更する
*/

#include <memory>


namespace beego {
  /*!
    \brief ログ管理クラス
  */
  class LogManager {
    LogManager(void);
    LogManager(const LogManager& rhs);
    LogManager& operator = (const LogManager& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    /*!
      \brief エラーレベル
    */
    typedef enum {
      NoWrite = 0,                //!< 全メッセージの無視
      Attack,                        //!< クラック
      Error,                        //!< エラー
      Warning,                        //!< 警告
      Notice,                        //!< 注意
      All,                        //!< 全メッセージの記録
    } ErrorLevel;

    ~LogManager(void);
    static LogManager* getObject(int id = 0);

    /*!
      \brief ログファイル名を指定する

      この関数は、最初に write() が呼ばれる前に呼び出されなければならない。

      \param fileName [i] ログファイル名
    */
    void setFileName(const char* fileName, int id = 0);


    /*!
      \brief ファイルに記録しないエラーレベルを指定する

      指定されたエラーレベルよりも低い記録をファイルに書き込まない。

      \param mask [i] 書き込みを行うエラーレベル

      \code
      LogManager* log = LogManager::getObject();
      log->setFileName("error_log.txt");

      // Warning, Notice はファイルに書き込まれなくなる
      log->setLogMask(Error);
      ...

      log->write(Error, "error!");        // 書き込まれる
      log->write(Warning, "warning!");         // 書き込まれない
      \endcode
    */
    void setLogMask(const ErrorLevel level, int id = 0);


    /*!
      \brief ログにメッセージを書き込む

      このメソッド毎にメッセージは改行されて記録される。

      \param level [i] ログレベル
      \param message [i] メッセージ
    */
    void write(const ErrorLevel level, const char* message);
  };
};

#endif /* !LOG_MANAGER_H */
