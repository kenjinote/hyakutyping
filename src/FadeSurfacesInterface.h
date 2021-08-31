#ifndef FADE_SURFACES_INTERFACE_H
#define FADE_SURFACES_INTERFACE_H

/*!
  \file
  \brief �t�F�[�h���ʗp�̃C���^�[�t�F�[�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include <boost/shared_ptr.hpp>
#include <vector>

namespace beego {
  class TextSurface;

  typedef std::vector<boost::shared_ptr<TextSurface> > FadeSurfaces;

  typedef enum {
    FadeInFromLeft,             //!< ������E�����ɕ\���
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
