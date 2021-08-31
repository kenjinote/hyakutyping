/*!
  \file
  \brief Component ��`

  \author Satofumi KAMIMURA

  $Id$

  \todo ComponentUtils �Ȃǂɉ������ׂ�
  \todo addDrawSurface() �Ȃǂ��A���̃t�@�C���Ɉړ�������A���H
*/

#include "Component.h"
#include "ComponentInterface.h"
#include "SurfaceInterface.h"
#include "SdlUtils.h"
#include "EvaluateArea.h"


// !!! ���Aarea �g���ĂȂ��ˁH
// !!! area, position �œn�������Ⴄ�̂��C������
void beego::updateDrawSurface(DrawSurface& draw_surface, Surface surface,
                              const SDL_Rect* area,
                              size_t ticks,
                              const SDL_Rect& position,
                              bool position_changed) {

  // !!! EvaluateArea �ł����Ēu��������A���ȁH

  // �R���|�[�l���g�̈�� area �̋��ʕ����� component_area �ƂȂ�
  draw_surface.surface = surface;
  if (! area) {
    // surface ���o�����ړI�̂Ƃ��́A�����Ŗ߂�
    return;
  }

  draw_surface.mustDraw = true;
  draw_surface.position = position;
  draw_surface.position_changed = position_changed;
  draw_surface.is_changed = surface->isChanged(ticks);
  draw_surface.is_transparent = surface->isTransparent();
  set_SdlRect(&draw_surface.area, 0, 0, position.w, position.h);
  //fprintf(stderr, "1 w:%d h:%d\n", draw_surface.area.w, draw_surface.area.h);
  //fprintf(stderr, "1 %d, %d\n", area->x, draw_surface.position.x);

  return;

#if 0
  // �ȉ��A����Ȃ��̂����B�R�����g�A�E�g���ėl�q���݂Ă݂�
  if (draw_surface.position.x < area->x) {
    int diff = area->x - draw_surface.position.x;
    draw_surface.position.x += diff;
    draw_surface.area.x = diff;
    draw_surface.area.w -= diff;
  }
  if (draw_surface.position.y < area->y) {
    int diff = area->y - draw_surface.position.y;
    draw_surface.position.y += diff;
    draw_surface.area.y = diff;
    draw_surface.area.h -= diff;
  }
  if (draw_surface.position.x + draw_surface.area.w > area->x + area->w) {
    draw_surface.area.w -=
      (draw_surface.position.x + draw_surface.area.w) - (area->x + area->w);
  }
  if (draw_surface.position.y + draw_surface.area.h > area->y + area->h) {
    draw_surface.area.h -=
      (draw_surface.position.y + draw_surface.area.h) - (area->y + area->h);
  }

  // �`��̈悪�Ȃ��Ȃ�����AmustDraw �� false �ɂ���
  // !!!
  // !!! �Ƃ�����L�̋L�q���A���ł��Ӗ�������̂��s��
  // !!! �C�����čČ�������
  fprintf(stderr, "2 w:%d h:%d\n", draw_surface.area.w, draw_surface.area.h);
#endif
}


extern bool beego::isAppliedRect(const int x, const int y,
                                 const std::vector<SDL_Rect>& applied_rects) {

  for (std::vector<SDL_Rect>::const_iterator it = applied_rects.begin();
       it != applied_rects.end(); ++it) {

    // �_�̈ʒu�����͔���ɗ��p�ς݂������� false ��Ԃ�
    if (isPointInside(x, y, *it)) {
      return false;
    }
  }

  // �ǂ̗̈�ł��g���Ă��Ȃ���΁Atrue ��Ԃ�
  return true;
}


int beego::leftPosition(Component component, int left_position) {
  return left_position;
}


int beego::centerPosition(Component component, int center_position) {
  return center_position - static_cast<int>(component->getWidth()/2);
}


int beego::rightPosition(Component component, int right_position) {
  return right_position - static_cast<int>(component->getWidth());
}


int beego::topPosition(Component component, int top_position) {
  return top_position;
}


int beego::middlePosition(Component component, int middle_position) {
  return middle_position - static_cast<int>(component->getHeight()/2);
}


int beego::bottomPosition(Component component, int bottom_position) {
  return bottom_position - static_cast<int>(component->getHeight());
}
