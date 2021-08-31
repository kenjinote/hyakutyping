#ifndef GET_TICKS_H
#define GET_TICKS_H

/*!
  \file
  \brief 時間間隔の取得

  \author Satofumi KAMIMURA

  $Id$

  \todo monitor 対応にする
*/

namespace beego {
  /*!
    \brief プログラム動作時間の取得

    \return プログラム動作時間 [msec]

    \attention 実際には、SDL_Init() が呼ばれてからの時間となる。
  */
  extern size_t GetTicks(void);
};

#endif /* !GET_TICKS_H */
