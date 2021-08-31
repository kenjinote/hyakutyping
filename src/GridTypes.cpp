/*!
  \file
  \brief グリッド型の定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include "GridTypes.h"
#include "MathUtils.h"

using namespace beego;


Angle::Angle(void) : radian(0.0) {
}


Angle::Angle(const Angle& rhs) : radian(rhs.radian) {
}


Angle& Angle::operator = (const Angle& rhs) {
  this->radian = rhs.radian;

  return *this;
}


Angle::~Angle(void) {
}


double Angle::to_rad(void) const {
  return radian;
}


double Angle::to_deg(void) const {
  return radian * 180.0 / M_PI;
}


//////////////////////////////////////////////////////////////////////
Angle beego::deg(const double degree) {

  Angle angle;
  angle.radian = degree * M_PI / 180.0;

  return angle;
}


Angle beego::rad(const double radian) {

  Angle angle;
  angle.radian = radian;

  return angle;
}
