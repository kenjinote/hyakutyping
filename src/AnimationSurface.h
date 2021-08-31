#ifndef ANIMATION_SURFACE_H
#define ANIMATION_SURFACE_H

/*!
  \file
  \brief Animation �T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  \todo �o�^�����C���f�b�N�X����A�j���[�V�������`���邩�A����������
*/

#include "SurfaceInterface.h"
#include "Surface.h"
#include <memory>


namespace beego {
  /*!
    \brief Animation �T�[�t�F�X�����N���X

    \attention �T�[�t�F�X�����̂��߂ɂ͉�ʃT�[�t�F�X�������K�v
  */
  class AnimationSurface : public SurfaceInterface {
    AnimationSurface(const AnimationSurface& rhs);
    AnimationSurface& operator = (const AnimationSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      Infinity = -1,            //!< ������̍Đ�
    };
    AnimationSurface(void);
    ~AnimationSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    // !!! void clear(void);
    void registerSurface(Surface surface, size_t elapse_msec,
                         int x_offset = 0, int y_offset = 0);
    void play(int times = Infinity);
  };
};

#endif /* !ANIMATION_SURFACE_H */
