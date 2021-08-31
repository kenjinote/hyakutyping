#ifndef COMPONENT_H
#define COMPONENT_H

/*!
  \file
  \brief Component ��`

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SurfaceList.h"

namespace beego {
  class ComponentInterface;

  /*!
    \brief �R���|�[�l���g�^�̒�`
  */
  typedef boost::shared_ptr<ComponentInterface> Component;

  extern void updateDrawSurface(DrawSurface& draw_surface,
                                Surface surface, const SDL_Rect* area,
                                size_t ticks,
                                const SDL_Rect& position,
                                bool position_changed);

  /*!
    \brief �_�̈ʒu�����͔���ɗ��p�ς݂��A�𔻒�
  */
  extern bool isAppliedRect(const int x, const int y,
                            const std::vector<SDL_Rect>& applied_rects);

  extern int leftPosition(Component component, int left_position);
  extern int centerPosition(Component component, int center_position);
  extern int rightPosition(Component component, int right_position);

  extern int topPosition(Component component, int top_position);
  extern int middlePosition(Component component, int middle_position);
  extern int bottomPosition(Component component, int bottom_position);
};

#endif /* !COMPONENT_H */
