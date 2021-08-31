#ifndef VOLUME_SLIDER_COMPONENT_H
#define VOLUME_SLIDER_COMPONENT_H

/*!
  \brief 音量スライダー

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>


namespace beego {
  class VolumeSliderComponent : public ComponentInterface {
    VolumeSliderComponent(const VolumeSliderComponent& rhs);
    VolumeSliderComponent& operator = (const VolumeSliderComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    VolumeSliderComponent(size_t width, size_t height);
    ~VolumeSliderComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! 分割数の設定
    int getValue(void);
    void setValue(int value);
    bool isChanged(void);
    // !!! 位置変更の許可、不許可
    // !!! 色の変化のあたりを指定
  };
};

#endif /* !VOLUME_SLIDER_COMPONENT_H */
