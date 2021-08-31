#ifndef COMBO_BOX_COMPONENT_H
#define COMBO_BOX_COMPONENT_H

/*!
  \file
  \brief �R���{�{�b�N�X�E�R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>
#include <string>


namespace beego {
  class ButtonComponent;
  class TextProperty;

  class ComboBoxComponent : public ComponentInterface {
    ComboBoxComponent(const ComboBoxComponent& rhs);
    ComboBoxComponent& operator = (const ComboBoxComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      NoItem = -1,              //!< ���ڂ��Ȃ�
    };
    ComboBoxComponent(void);
    ~ComboBoxComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    void setRightOffset(size_t width);
    void setMinWidth(size_t width);
    void clearItems(void);
    void addItem(boost::shared_ptr<ButtonComponent> item);
    void setSelected(int index);
    int getSelected(void);
    bool isSelectChanged(void);

    // !!! �F�̎w��́AaddItem() ���Ăяo���O�ɍs���ׂ�
    void setFrameColor(Uint32 color);

    // !!! �F�̎w��́AaddItem() ���Ăяo���O�ɍs���ׂ�
    void setDecidedBackgroundColor(Uint32 color);

    // !!! �F�̎w��́AaddItem() ���Ăяo���O�ɍs���ׂ�
    void setSelectedBackgroundColor(Uint32 coolor);

    // !!! system_color �͘g�̐F
    static void createComboBox(boost::shared_ptr<ComboBoxComponent>& combobox,
                               std::vector<std::string>& texts,
                               const TextProperty& normal,
                               const TextProperty& selected,
                               Uint32 frame_color);
  };
};

#endif /* !COMBO_BOX_COMPONENT_H */
