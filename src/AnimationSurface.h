#ifndef ANIMATION_SURFACE_H
#define ANIMATION_SURFACE_H

/*!
  \file
  \brief Animation サーフェス

  \author Satofumi KAMIMURA

  $Id$

  \todo 登録したインデックスからアニメーションを定義するか、を検討する
*/

#include "SurfaceInterface.h"
#include "Surface.h"
#include <memory>


namespace beego {
  /*!
    \brief Animation サーフェス処理クラス

    \attention サーフェス処理のためには画面サーフェス生成が必要
  */
  class AnimationSurface : public SurfaceInterface {
    AnimationSurface(const AnimationSurface& rhs);
    AnimationSurface& operator = (const AnimationSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      Infinity = -1,            //!< 無限回の再生
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
