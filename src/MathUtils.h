#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/*!
  \file
  \brief 数学関数の補助定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include "DetectOS.h"
#include <math.h>


#ifdef WINDOWS_OS
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#endif

#endif /* !MATH_UTILS_H */
