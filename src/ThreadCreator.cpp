/*!
  \file
  \brief SDL_Thread によるスレッドハンドラ

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ThreadCreator.h"
#include "SdlInit.h"
#include "LockGuard.h"
#include <SDL_thread.h>

using namespace beego;


/*!
  \brief ThreadCreator の内部クラス
*/
struct ThreadCreator::pImpl : private SdlInit {
  static int function_handler(void* args);

  /*!
    \brief ThreadCreator の内部クラス
  */
  class ThreadInfo {
  public:
    SDL_mutex* mutex;
    int (*function)(void *);
    void* args;
    bool pause;
    bool quit;
    size_t times;
    size_t next_times;

    ThreadInfo(int (*fn)(void *), void* thread_args)
      : mutex(SDL_CreateMutex()), function(fn), args(thread_args),
        pause(true), quit(false), times(1), next_times(0) {
    }

    ~ThreadInfo(void) {
      SDL_DestroyMutex(mutex);
    }
  };

  SDL_Thread* thread;
  ThreadInfo thread_info;

  pImpl(int (*fn)(void *), void* args)
    : thread(NULL), thread_info(fn, args) {
  }

  ~pImpl(void) {
    thread_info.quit = true;
    if (thread) {
      SDL_KillThread(thread);
    }
  }
};


/*!
  \todo pause 変数の使い方を見直すべき
*/
int ThreadCreator::pImpl::function_handler(void* args) {
  pImpl::ThreadInfo& info = *(pImpl::ThreadInfo *)args;

  int ret = 0;
  bool pause = false;
  bool quit = false;
  while (! quit) {
    if (pause) {
      SDL_Delay(1);
    } else {
      ret = info.function(info.args);
    }

    LockGuard guard(info.mutex);
    if (pause == false) {
      ++info.times;
    }

    pause = info.pause;
    quit = info.quit;
    if (info.times == info.next_times) {
      // !!! pause 変数の使い方を見直すべき
      info.pause = true;
      pause = true;
    }
  }
  return ret;
}


ThreadCreator::ThreadCreator(int (*fn)(void *), void* args)
  : pimpl(new pImpl(fn, args)) {
}


ThreadCreator::~ThreadCreator(void) {
}


void ThreadCreator::run(void) {
  if (pimpl->thread == NULL) {
    // スレッドの起動
    pimpl->thread_info.pause = false;
    pimpl->thread = SDL_CreateThread(pImpl::function_handler,
                                     &pimpl->thread_info);
    return;
  }

  // スレッドの再開
  LockGuard guard(pimpl->thread_info.mutex);
  pimpl->thread_info.pause = false;
}


void ThreadCreator::run(size_t times) {
  LockMutex(pimpl->thread_info.mutex);
  size_t now_times = pimpl->thread_info.times;
  pimpl->thread_info.next_times = now_times + times;
  UnlockMutex(pimpl->thread_info.mutex);

  run();
}


void ThreadCreator::stop(void) {
  // スレッドの停止
  LockGuard guard(pimpl->thread_info.mutex);
  pimpl->thread_info.pause = true;
}


void ThreadCreator::wait(int* retval) {
  pimpl->thread_info.quit = true;
  SDL_WaitThread(pimpl->thread, retval);
  pimpl->thread = NULL;
}


bool ThreadCreator::isRunning(void) {
  return ((pimpl->thread == NULL) || pimpl->thread_info.pause) ? false : true;
}
