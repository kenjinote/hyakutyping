#ifndef LOCK_GUARD_H
#define LOCK_GUARD_H

/*!
  \file
  \brief SDL_mutex �̃��b�N�K�[�h�N���X

  \author Satofumi KAMIMURA

  $Id$

  \todo SDL_mutex �����b�v���ׂ�������������B���Atypedef ���邾�����H �ނ�
*/

#ifdef MSC
#include "SDL_win.h"
#else
#include <SDL.h>
#endif
#include <memory>


namespace beego {
  /*!
    \brief SDL_mutex �̃��b�N�K�[�h�N���X
  */
  class LockGuard {
    LockGuard(void);
    LockGuard(const LockGuard& rhs);
    LockGuard& operator = (const LockGuard& rhs);

    void* operator new (size_t);
    void* operator new[] (size_t);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit LockGuard(SDL_mutex* mutex);
    ~LockGuard(void);
  };

  extern void LockMutex(SDL_mutex* mutex);
  extern void UnlockMutex(SDL_mutex* mutex);
};

#endif /* !LOCK_GUARD_H */
