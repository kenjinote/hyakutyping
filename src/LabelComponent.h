#ifndef LABEL_COMPONENT_H
#define LABEL_COMPONENT_H

/*!
  \file
  \brief Label コンポーネント

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  /*!
    \brief Label コンポーネント処理クラス

    \todo LogManager 対応にする
  */
  class LabelComponent : public ComponentInterface {
    LabelComponent(void);
    LabelComponent(const LabelComponent& rhs);
    LabelComponent& operator = (const LabelComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit LabelComponent(Surface surface);
    ~LabelComponent(void);

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

#endif /* !LABEL_COMPONENT_H */
