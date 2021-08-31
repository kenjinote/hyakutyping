/*!
  \file
  \brief ��T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  Surface �� NULL ���w�肷�����ɂ��̃T�[�t�F�X���g��
*/

#include "NullSurface.h"

using namespace beego;


NullSurface::NullSurface(void) {
}


NullSurface::~NullSurface(void) {
}


void NullSurface::draw(std::vector<SDL_Rect>& update_rects,
                       const SDL_Rect* pos, const SDL_Rect* area) {
  // �������Ȃ�
}


size_t NullSurface::getWidth(void) {
  return 0;
}


size_t NullSurface::getHeight(void) {
  return 0;
}


void NullSurface::forceSetChanged(void) {
  // �������Ȃ�
}


bool NullSurface::isChanged(size_t ticks) {
  return false;
}


bool NullSurface::isTransparent(void) {
  return false;
}
