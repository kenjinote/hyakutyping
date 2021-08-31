/*!
  \file
  \brief �ĕ`��̂��߂̃T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "RedrawSurface.h"

using namespace beego;


struct RedrawSurface::pImpl {
  SDL_Rect redraw_rect_;
};


RedrawSurface::RedrawSurface(void) : pimpl(new pImpl) {
}


RedrawSurface::~RedrawSurface(void) {
}


void RedrawSurface::draw(std::vector<SDL_Rect>& update_rects,
                         const SDL_Rect* pos, const SDL_Rect* area) {

  // ���ۂ̕`��͍s��Ȃ�
}


size_t RedrawSurface::getWidth(void) {
  return pimpl->redraw_rect_.w;
}


size_t RedrawSurface::getHeight(void) {
  return pimpl->redraw_rect_.h;
}


void RedrawSurface::forceSetChanged(void) {
  // ��� isChanged �� true ��Ԃ����߁A�������Ȃ��Ă悢
}


bool RedrawSurface::isChanged(size_t ticks) {
  return true;
}


bool RedrawSurface::isTransparent(void) {
  return true;
}


void RedrawSurface::setRedrawRect(SDL_Rect& rect) {
  pimpl->redraw_rect_ = rect;
}
