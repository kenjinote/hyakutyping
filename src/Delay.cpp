/*!
  \file
  \brief 遅延時間を制御可能な delay() 関数

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
  \brief SdlInit の初期化クラス
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
  \brief 待機関数

  指定時間だけ待機する。

  \param msec [i] 待機時間 [msec]
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
  \brief 時間の進行比率を設定

  モニタ機能にて、早送りなどを実現するのに利用する。

  \param ratio [i] 時間のすすみ具合。2.0 だと２倍速
*/
void setDelayRatio(double ratio) {
  mutexInitialize();

  LockMutex(Mutex);
  WaitRatio = ratio;
  UnlockMutex(Mutex);
}
