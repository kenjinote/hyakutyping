#ifndef EVALUATE_AREA_H
#define EVALUATE_AREA_H

/*!
  \file
  \brief ãÈå`ÇÃèdÇ»ÇËÇï]âø

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


namespace beego {
  void evaluateArea(std::vector<SDL_Rect>& common_area,
                    std::vector<SDL_Rect>& unique_area,
                    const SDL_Rect& base_area, const SDL_Rect& compare_area);

  bool isPointInside(int px, int py, const SDL_Rect& area);
};

#endif /* !EVALUATE_AREA_H */
