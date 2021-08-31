#ifndef SURFACE_H
#define SURFACE_H

/*!
  \file
  \brief Surface ��`

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/shared_ptr.hpp>

namespace beego {
  class SurfaceInterface;

  /*!
    \brief �T�[�t�F�X�^�̒�`
  */
  typedef boost::shared_ptr<SurfaceInterface> Surface;
};

#endif /* !SURFACE_H */
