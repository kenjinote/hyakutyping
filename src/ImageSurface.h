#ifndef IMAGE_SURFACE_H
#define IMAGE_SURFACE_H

/*!
  \file
  \brief �摜�T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  /*!
    \brief �摜�T�[�t�F�X�����N���X

    \attention �T�[�t�F�X�����̂��߂ɂ͉�ʃT�[�t�F�X�������K�v
  */
  class ImageSurface : public SurfaceInterface {
    ImageSurface(void);
    ImageSurface(const ImageSurface& rhs);
    ImageSurface& operator = (const ImageSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit ImageSurface(const char* imageFile, bool transparent = false);
    ~ImageSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);
  };
};

#endif /* !IMAGE_SURFACE_H */
