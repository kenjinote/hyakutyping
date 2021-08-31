/*!
  \file
  \brief �x�����Ԃ𐧌�\�� delay() �֐�

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Delay.h"
#include "SdlInit.h"
#include "LockGuard.h"
#include "DetectOS.h"


using namespace beego;


static double WaitRatio = 1.0;
static SDL_mutex* Mutex = NULL;

/*!
  \brief SdlInit �̏������N���X
*/
class SdlInitClass : private SdlInit {
};

static void mutexInitialize(void) {
  static SdlInitClass sdl_init;
  static SDL_mutex* obj = SDL_CreateMutex();
  if (Mutex == NULL) {
    Mutex = obj;
  }
}


/*!
  \brief �ҋ@�֐�

  �w�莞�Ԃ����ҋ@����B

  \param msec [i] �ҋ@���� [msec]
*/
void beego::delay(size_t msec) {
  mutexInitialize();

  LockMutex(Mutex);
  unsigned long delay_msec =
    static_cast<unsigned long>(msec * WaitRatio);
  UnlockMutex(Mutex);

  SDL_Delay(delay_msec);
}


/*!
  \brief ���Ԃ̐i�s�䗦��ݒ�

  ���j�^�@�\�ɂāA������Ȃǂ���������̂ɗ��p����B

  \param ratio [i] ���Ԃ̂����݋�B2.0 ���ƂQ�{��
*/
void setDelayRatio(double ratio) {
  mutexInitialize();

  LockMutex(Mutex);
  WaitRatio = ratio;
  UnlockMutex(Mutex);
}
