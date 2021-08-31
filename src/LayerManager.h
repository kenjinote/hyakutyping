#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

/*!
  \file
  \brief レイヤー管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentList.h"
#include <memory>

namespace beego {
  class Layer;
  class InputEvent;

  /*!
    \brief 描画レイヤー管理
  */
  class LayerManager {
    LayerManager(void);
    LayerManager(const LayerManager& rhs);
    LayerManager& operator = (const LayerManager& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    LayerManager(SurfaceList& surface_list);
    ~LayerManager(void);

    void updateScreenSurface(SDL_Surface* scr);
    void applyInput(ComponentList& components, const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void redraw(std::vector<SDL_Rect>& update_rects,
                SurfaceList& surfaces);
    void push_front(Layer* layer);
    void push_back(Layer* layer);
    void insert(const Layer* dst, Layer* layer);
    void remove(Layer* layer);
    void createComponentList(ComponentList& components);
    void createSurfaceList(SurfaceList& surfaces,
                           const ComponentList& components, size_t ticks);
    void removeSurface(SurfaceList& remove_list);
  };
};

#endif /* !LAYER_MANAGER_H */
