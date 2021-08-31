#ifndef MENU_COMPONENT_H
#define MENU_COMPONENT_H

/*!
  \file
  \brief ���j���[�R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$

  \todo �{�^������邽�߂̃��\�b�h��ǉ�����
*/

#include "ComponentInterface.h"
#include "PlaceOption.h"
#include "Component.h"
#include <memory>

namespace beego {
  class ButtonComponent;

  class MenuComponent : public ComponentInterface {
    MenuComponent(const MenuComponent& rhs);
    MenuComponent& operator = (const MenuComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      NoSelect = -1,
    };
    MenuComponent(void);
    ~MenuComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    void clearItems(void);
    void addItem(boost::shared_ptr<ButtonComponent> item,
                 int x_offset = 0, int y_offset = 0);
    void setItemsOffset(int x_offset, int y_offset);
    void getItemPosition(SDL_Rect* offset, size_t index);
    void adjustItemsVerticalOffset(size_t additional_offset = 0);
    void adjustItemsHorizontalOffset(size_t additional_offset = 0);
    void setSelected(int index);
    void setDecided(int index);
    int getSelected(void);
    int getDecided(void);
    void releaseDecided(void);

    // �I�𕝂��A�w�蕝�܂Ŋg������
    void setItemSelectWidth(size_t width);

    // !!! �A�C�R���T�[�t�F�X�̓o�^�A���H
    // !!! �A�C�R����`�悷�ׂ��ʒu��Ԃ��΁A����łȂ�Ƃ��Ȃ�C�����邪...
    // !!! ���̕����A�Ɨ����͍������Ȃ�...
    // !!! �܂��A�o�^���郁�\�b�h�����󂾂�
    void setIconComponent(Component icon,
                          int x_offset, int y_offset, int mode = None);
  };
};

#endif /* !MENU_COMPONENT_H */
