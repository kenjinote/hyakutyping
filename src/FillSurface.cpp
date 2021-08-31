/*!
  \file
  \brief �~���l�߃T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  \todo OpenGL �łŃT�[�t�F�X���k������ĕ\�������̂��A���Ƃ�����
*/

#include "FillSurface.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"

using namespace beego;


struct FillSurface::pImpl {
  Surface surface;
  size_t width;
  size_t height;
  size_t x_offset;
  size_t y_offset;
  bool is_changed;

  pImpl(Surface obj, size_t surface_width, size_t surface_height,
        size_t surface_x_offset, size_t surface_y_offset)
    : surface(obj), width(surface_width), height(surface_height),
      x_offset(surface_x_offset), y_offset(surface_y_offset),
      is_changed(false) {
  }
};


FillSurface::FillSurface(Surface surface, size_t width, size_t height,
                         size_t x_offset, size_t y_offset)
  : pimpl(new pImpl(surface, width, height, x_offset, y_offset)) {
}


FillSurface::~FillSurface(void) {
}


void FillSurface::draw(std::vector<SDL_Rect>& update_rects,
                       const SDL_Rect* pos, const SDL_Rect* area) {

  size_t surface_width = pimpl->surface->getWidth();
  size_t surface_height = pimpl->surface->getHeight();
  //fprintf(stderr, "%d, %d\n", surface_width, surface_height);

  // !!! �Ƃ肠�����A���Ɏ��܂�����\�������Ă݂�
  // !!! offset �̒������A�܂�

  // !!! �Ƃ������Aarea ���� for �̏����͌v�Z�����ׂ�����
  std::vector<SDL_Rect> common_area;
  std::vector<SDL_Rect> unique_area;
  int y_offset = -(area->y % static_cast<int>(surface_height));
  int x_offset = -(area->x % static_cast<int>(surface_width));

  SDL_Rect eval_area;
  set_SdlRect(&eval_area, pos->x, pos->y, area->w, area->h);

  //fprintf(stderr, "x_offset: %d\n", x_offset);
  for (int h = y_offset; h < static_cast<int>(pimpl->height);
       h += static_cast<int>(surface_height)) {
    for (int w = x_offset; w < static_cast<int>(pimpl->width);
         w += static_cast<int>(surface_width)) {
      SDL_Rect eval_pos;
      set_SdlRect(&eval_pos, pos->x + w, pos->y + h,
                  static_cast<int>(surface_width),
                  static_cast<int>(surface_height));

      // !!! draw_area ���`�������A�����p���čĕ`����s��
      // !!! for �ŌJ��Ԃ��͈͂��ς��̂Œ��ӂ��邱��

      // !!! area ����͂ݏo�Ă���ӏ��ɂ��ẮA�`�悵�Ȃ��悤�ɂ���
      // !!! ���́A�͂ݏo�ĂĂ��A�C�ɂ����`�悵�Ă�

      common_area.clear();
      evaluateArea(common_area, unique_area, eval_pos, eval_area);
      if (! common_area.empty()) {
        SDL_Rect& common = common_area.front();
        SDL_Rect draw_area;
        set_SdlRect(&draw_area, common.x - eval_pos.x, common.y - eval_pos.y,
                    common.w, common.h);
        pimpl->surface->draw(update_rects, &common, &draw_area);
      }
    }
  }
}


size_t FillSurface::getWidth(void) {
  return pimpl->width;
}


size_t FillSurface::getHeight(void) {
  return pimpl->height;
}


void FillSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool FillSurface::isChanged(size_t ticks) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool FillSurface::isTransparent(void) {
  return pimpl->surface->isTransparent();
}
