#ifndef SURFACE_LIST_H
#define SURFACE_LIST_H

/*!
  \file
  \brief �T�[�t�F�X���X�g

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Surface.h"
#include <SDL.h>
#include <vector>
#include <list>


namespace beego {
  /*!
    \brief �`��T�[�t�F�X�̊Ǘ�
  */
  class DrawSurface {
    static int serial_id;

  public:
    bool mustDraw;              //!< �`�悷��K�v���̗L��
    SDL_Rect position;          //!< �ʒu
    SDL_Rect area;              //!< �I�t�Z�b�g�A�T�C�Y
    Surface surface;            //!< �T�[�t�F�X
    bool position_changed;      //!< �z�u���ύX���ꂽ��
    bool is_changed;
    bool is_transparent;
    std::vector<SDL_Rect> redraw_rects; //!< �ĕ`��̈�
  };

  /*!
    \brief �`��T�[�t�F�X���X�g
  */
  typedef std::list<DrawSurface*> SurfaceList;
};

#endif /* !SURFACE_LIST_H */
