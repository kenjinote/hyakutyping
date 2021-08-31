#ifndef FADE_SURFACES_INTERFACE_H
#define FADE_SURFACES_INTERFACE_H

/*!
  \file
  \brief フェード効果用のインターフェース

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/shared_ptr.hpp>
#include <vector>

namespace beego {
  class TextSurface;

  typedef std::vector<boost::shared_ptr<TextSurface> > FadeSurfaces;

  typedef enum {
    FadeInFromLeft,             //!< 左から右方向に表れる
  } FadeMode;

  class FadeSurfacesInterface {
  public:
    virtual ~FadeSurfacesInterface(void) {}

    virtual FadeSurfaces& getSurfaces(void) = 0;
    virtual int getPositionPercent(size_t index) = 0;
    virtual size_t getWidth(void) = 0;
    virtual size_t getHeight(void) = 0;
  };
};

#endif /* !FADE_SURFACES_INTERFACE_H */
