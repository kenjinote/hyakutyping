#ifndef SWITCH_LABEL_COMPONENT_H
#define SWITCH_LABEL_COMPONENT_H

/*!
  \file
  \brief •\Ž¦‚ð“ü‚ê‘Ö‚¦‰Â”\‚È LabelComponent

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  class SwitchSurface;

  class SwitchLabelComponent : public ComponentInterface {
    SwitchLabelComponent(void);
    SwitchLabelComponent(const SwitchLabelComponent& rhs);
    SwitchLabelComponent& operator = (const SwitchLabelComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      None = 0x00,
      Top = 0x01,
      Middle = 0x02,
      Bottom = 0x04,
      Left = 0x08,
      Center = 0x10,
      Right = 0x20,
    };
    explicit SwitchLabelComponent(boost::shared_ptr<SwitchSurface> surface,
                                  int mode = None);
    ~SwitchLabelComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);
  };
};

#endif /* !SWITCH_LABEL_COMPONENT_H */
