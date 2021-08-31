#ifndef FILL_SURFACE_H
#define FILL_SURFACE_H

/*!
  \file
  \brief �~���l�߃T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Surface.h"
#include "SurfaceInterface.h"
#include <memory>


namespace beego {
  /*!
    \brief �h��Ԃ��T�[�t�F�X�����N���X

    \attention �T�[�t�F�X�����̂��߂ɂ͉�ʃT�[�t�F�X�������K�v

    \todo pos, area �̊֌W���C������
  */
  class FillSurface : public SurfaceInterface {
    FillSurface(const FillSurface& rhs);
    FillSurface& operator = (const FillSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit FillSurface(Surface surface, size_t width, size_t height,
                         size_t x_offset = 0, size_t y_offset = 0);
    ~FillSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);
  };
};

#endif /* !FILL_SURFACE_H */
