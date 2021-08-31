/*!
  \file
  \brief Component 定義

  \author Satofumi KAMIMURA

  $Id$

  \todo ComponentUtils などに改名すべき
  \todo addDrawSurface() などを、他のファイルに移動させる、か？
*/

#include "Component.h"
#include "ComponentInterface.h"
#include "SurfaceInterface.h"
#include "SdlUtils.h"
#include "EvaluateArea.h"


// !!! つか、area 使ってなくね？
// !!! area, position で渡し方が違うのを修正する
void beego::updateDrawSurface(DrawSurface& draw_surface, Surface surface,
                              const SDL_Rect* area,
                              size_t ticks,
                              const SDL_Rect& position,
                              bool position_changed) {

  // !!! EvaluateArea でもって置き換える、かな？

  // コンポーネント領域と area の共通部分が component_area となる
  draw_surface.surface = surface;
  if (! area) {
    // surface 取り出しが目的のときは、ここで戻る
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
  // 以下、いらないのかも。コメントアウトして様子をみてみる
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

  // 描画領域がなくなったら、mustDraw を false にする
  // !!!
  // !!! という上記の記述が、今でも意味があるのか不明
  // !!! 気を見て再検討する
  fprintf(stderr, "2 w:%d h:%d\n", draw_surface.area.w, draw_surface.area.h);
#endif
}


extern bool beego::isAppliedRect(const int x, const int y,
                                 const std::vector<SDL_Rect>& applied_rects) {

  for (std::vector<SDL_Rect>::const_iterator it = applied_rects.begin();
       it != applied_rects.end(); ++it) {

    // 点の位置が入力判定に利用済みだったら false を返す
    if (isPointInside(x, y, *it)) {
      return false;
    }
  }

  // どの領域でも使われていなければ、true を返す
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
