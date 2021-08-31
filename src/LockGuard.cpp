/*!
  \file
  \brief SDL_mutex �̃��b�N�K�[�h�N���X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "LockGuard.h"

using namespace beego;


/*!
  \brief LockGuard �̓����N���X
*/
struct LockGuard::pImpl {
  SDL_mutex* mutex_;

  pImpl(SDL_mutex* mutex) : mutex_(mutex) {
  }
};


LockGuard::LockGuard(SDL_mutex* mutex) : pimpl(new pImpl(mutex)) {
  SDL_LockMutex(pimpl->mutex_);
}


LockGuard::~LockGuard(void) {
  SDL_UnlockMutex(pimpl->mutex_);
}


void beego::LockMutex(SDL_mutex* mutex) {
  SDL_LockMutex(mutex);
}


void beego::UnlockMutex(SDL_mutex* mutex) {
  SDL_UnlockMutex(mutex);
}
