#ifndef CREATE_SURFACE_FROM_ARRAY_H
#define CREATE_SURFACE_FROM_ARRAY_H

/*!
  \file
  \brief �w�b�_�t�@�C������̃T�[�t�F�X�쐬

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>


namespace beego {
  extern SDL_Surface* createSurface(const unsigned char data[],
                                    int width, int height);
};

#endif /* !CREATE_SURFACE_FROM_ARRAY_H */
