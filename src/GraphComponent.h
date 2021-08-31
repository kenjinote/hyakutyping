#ifndef GRAPH_COMPONENT_H
#define GRAPH_COMPONENT_H

/*!
  \file
  \brief グラフ描画

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include "GridTypes.h"
#include <memory>


namespace beego {
  class TtfResource;
  class TextProperty;

  class GraphComponent : public ComponentInterface {
    GraphComponent(const GraphComponent& rhs);
    GraphComponent& operator = (const GraphComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    GraphComponent(void);
    ~GraphComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! 指定するまでは、透明
    void setBackgroundColor(Uint32 color);
    void setForeColor(Uint32 color);

    void drawTitle(const TextProperty& title);
    void registerLine(std::vector<Grid<double> >& points, Uint32 color,
                      TextProperty* text = NULL);
    void create(size_t width, size_t height, TtfResource* font);
    void createLegend(int x, int y, Uint32 fore, Uint32 back);
    void clear(void);
  };
};

#endif /* !GRAPH_COMPONENT_H */
