#ifndef SURFACE_H
#define SURFACE_H

/*!
  \file
  \brief Surface 定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/shared_ptr.hpp>

namespace beego {
  class SurfaceInterface;

  /*!
    \brief サーフェス型の定義
  */
  typedef boost::shared_ptr<SurfaceInterface> Surface;
};

#endif /* !SURFACE_H */
