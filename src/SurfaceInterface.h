#ifndef SURFACE_INTERFACE_H
#define SURFACE_INTERFACE_H

/*!
  \file
  \brief �T�[�t�F�X�C���^�[�t�F�[�X

  \author Satofumi KAMIMURA

  $Id$

  �T�[�t�F�X�Ƃ́A�`�掑�����Ǘ��������

  \todo ticks �ŕ`����e���ς��ꍇ�́A�S�̂̍ĕ`�悪�K�v�����B��������
  \todo �폜�p�ɁA�`�悵���Ƃ��� ticks ���P�v�I�ɋL�^����d�g�݂��K�v����������
*/

#include <SDL.h>
#include <vector>


namespace beego {
  /*!
    \brief �`��T�[�t�F�X�̃C���^�[�t�F�[�X��`
  */
  class SurfaceInterface {
  public:
    virtual ~SurfaceInterface() {}

    /*!
      \brief �T�[�t�F�X�̎��`��

      \param pos [i] �`��ʒu
      \param area [i] �`��I�t�Z�b�g�A����ѕ`��T�C�Y

      �`��ʒu���n�_�Ƃ��āA�`��I�t�Z�b�g����`��T�C�Y�����`�悷��
    */
    virtual void draw(std::vector<SDL_Rect>& update_rects,
                      const SDL_Rect* pos, const SDL_Rect* area) = 0;
    virtual size_t getWidth(void) = 0;
    virtual size_t getHeight(void) = 0;
    virtual void forceSetChanged(void) = 0;
    virtual bool isChanged(size_t ticks) = 0;
    virtual bool isTransparent(void) = 0;
  };
};

#endif /* !SURFACE_INTERFACE_H */
