#ifndef DELAY_H
#define DELAY_H

/*!
  \file
  \brief �x�����Ԃ𐧌�\�� delay() �֐�

  \author Satofumi KAMIMURA

  $Id$
*/

#include <stddef.h>


namespace beego {
  extern void delay(size_t msec);
  extern void setDelayRatio(double ratio);
};

#endif /* !DELAY_H */
