/*!
  \file
  \brief Animation サーフェス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "AnimationSurface.h"
#include "SwitchSurface.h"

using namespace beego;


struct AnimationSurface::pImpl {

  SwitchSurface surfaces_;
  bool is_transparent_;
  bool is_changed_;
  size_t index_;
  size_t width_;
  size_t height_;
  size_t total_elapse_msec_;
  std::vector<size_t> elapse_msec_list_;
  size_t first_ticks_;
  size_t left_times_;
  bool activate_;

  pImpl(void)
    : is_transparent_(false), is_changed_(false),
      index_(0), width_(0), height_(0), total_elapse_msec_(0),
      first_ticks_(false), left_times_(0), activate_(false) {
  }

  void clear(void) {
    elapse_msec_list_.clear();
  }

  void registerSurface(Surface surface, size_t elapse_msec,
                       int x_offset, int y_offset) {

    // アニメーション用のサーフェスを登録
    surfaces_.registerSurface(surface, x_offset, y_offset, index_);
    ++index_;
    elapse_msec_list_.push_back(elapse_msec);
    total_elapse_msec_ += elapse_msec;

    // 最大サイズの拡張
    size_t w = x_offset + surface->getWidth();
    if (w > width_) {
      width_ = w;
    }

    size_t h = y_offset + surface->getHeight();
    if (h > height_) {
      height_ = h;
    }

    // １枚でも透過サーフェスがあれば、透過とみなす
    is_transparent_ |= surface->isTransparent();
  }

  void draw(std::vector<SDL_Rect>& update_rects,
            const SDL_Rect* pos, const SDL_Rect* area) {
    if (surfaces_.empty()) {
      return;
    }
    surfaces_.draw(update_rects, pos, area);
  }

  bool isChanged(size_t ticks) {
    if (surfaces_.empty()) {
      return false;
    }

    if (activate_) {
      activate_ = false;

      // 再生開始 ticks の記録
      first_ticks_ = ticks;
    }

    // 表示セルの決定
    size_t cell_ticks = (ticks - first_ticks_) % total_elapse_msec_;
    size_t evaluate_ticks = 0;
    size_t draw_index = 0;
    for (std::vector<size_t>::iterator it = elapse_msec_list_.begin();
         it != elapse_msec_list_.end(); ++it) {
      evaluate_ticks += *it;
      if (evaluate_ticks > cell_ticks) {
        break;
      }
      ++draw_index;
    }

    bool ret = is_changed_;
    is_changed_ = false;

    size_t now_index = surfaces_.getSurfaceIndex();
    if (draw_index != now_index) {
      surfaces_.switchSurface(draw_index);
      ret |= true;
    }
    return ret;
  }
};


AnimationSurface::AnimationSurface(void) : pimpl(new pImpl) {
}


AnimationSurface::~AnimationSurface(void) {
}


void AnimationSurface::draw(std::vector<SDL_Rect>& update_rects,
                            const SDL_Rect* pos, const SDL_Rect* area) {
  pimpl->draw(update_rects, pos, area);
}


size_t AnimationSurface::getWidth(void) {
  return pimpl->width_;
}


size_t AnimationSurface::getHeight(void) {
  return pimpl->height_;
}


void AnimationSurface::forceSetChanged(void) {
  pimpl->is_changed_ = true;
}


bool AnimationSurface::isChanged(size_t ticks) {
  return pimpl->isChanged(ticks);
}


bool AnimationSurface::isTransparent(void) {
  return pimpl->is_transparent_;
}


void AnimationSurface::registerSurface(Surface surface, size_t elapse_msec,
                                       int x_offset, int y_offset) {
  pimpl->registerSurface(surface, elapse_msec, x_offset, y_offset);
}


void AnimationSurface::play(int times) {

  // アニメーション再生の開始
  pimpl->left_times_ = times;
  pimpl->activate_ = true;
}
