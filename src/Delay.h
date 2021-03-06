#ifndef DELAY_H
#define DELAY_H

/*!
  \file
  \brief 遅延時間を制御可能な delay() 関数

  \author Satofumi KAMIMURA

  $Id$
*/

#include <stddef.h>


namespace beego {
  extern void delay(size_t msec);
  extern void setDelayRatio(double ratio);
};

#endif /* !DELAY_H */
