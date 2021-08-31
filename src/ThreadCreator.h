#ifndef THREAD_CREATOR_SDL_H
#define THREAD_CREATOR_SDL_H

/*!
  \file
  \brief SDL_Thread によるスレッドハンドラ

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief スレッドの生成クラス
  */
  class ThreadCreator {
    ThreadCreator(void);
    ThreadCreator(const ThreadCreator& rhs);
    ThreadCreator& operator = (const ThreadCreator& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit ThreadCreator(int (*fn)(void *), void* args);
    ~ThreadCreator(void);

    /*!
      \brief スレッドの起動
    */
    void run(void);

    /*!
      \brief 処理回数を指定してスレッドを起動

      \param times [i] 処理回数
    */
    void run(size_t times);

    /*!
      \brief スレッドを停止
    */
    void stop(void);

    /*!
      \brief スレッドの終了を待つ

      \param retval [o] スレッドの戻り値が格納される
    */
    void wait(int* retval = NULL);

    /*!
      \brief スレッドが動作中かを返す

      \retval true 動作中
      \retval false 停止中
    */
    bool isRunning(void);
  };
};

#endif /* !THREAD_CREATOR_SDL_H */
