#ifndef COMPONENT_INTERFACE_H
#define COMPONENT_INTERFACE_H

/*!
  \file
  \brief �R���|�[�l���g�C���^�[�t�F�[�X

  \author Satofumi KAMIMURA

  $Id$

  �R���|�[�l���g�Ƃ́A�@�\�̎������Ǘ�������� (�����s������...)
*/

#include "SurfaceList.h"

namespace beego {
  class InputEvent;
  class Layer;

  /*!
    \brief �R���|�[�l���g�C���^�[�t�F�[�X
  */
  class ComponentInterface {
  public:
    virtual ~ComponentInterface() {}
    virtual void addSurfaceList(SurfaceList& surfaces,
                                const SDL_Rect* area, size_t ticks) = 0;
    virtual void setPosition(const SDL_Rect* position) = 0;

    // !!! SDL_Rect getPosition(void) �ɕύX���邩�ȁH
    virtual void getPosition(SDL_Rect* position) = 0;

    virtual size_t getWidth(void) = 0;
    virtual size_t getHeight(void) = 0;
    virtual void applyInput(const InputEvent& event,
                            std::vector<SDL_Rect>& applied_rects) = 0;

    // !!! �u�z�u�v�̈Ӗ����܂ރ��\�b�h���ɒu��������
    // !!! �Ƃ��������͕ۗ�
    virtual void registerLayer(Layer* layer) = 0;
  };
};

#endif /* !COMPONENT_INTERFACE_H */
