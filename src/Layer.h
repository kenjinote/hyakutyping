#ifndef LAYER_H
#define LAYER_H

/*!
  \file
  \brief レイヤー

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentList.h"
#include <memory>

namespace beego {
  class LayerManager;

  /*!
    \brief 描画レイヤー
  */
  class Layer {
    Layer(const Layer& rhs);
    Layer& operator = (const Layer& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    Layer(void);
    ~Layer(void);

    void addComponentList(ComponentList& components) const;
    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void push_front(Component component);
    void push_back(Component component);
    void insert(const Component dst, Component component);
    void remove(Component component);

    void removeSurfaces(SurfaceList& surfaces);
    void registerManager(LayerManager* obj);

    void enable(void);
    void disable(void);
  };
};

#endif /* !LAYER_H */
