#ifndef GUI_COLORS_H
#define GUI_COLORS_H

/*!
  \file
  \brief �F�̒�`

  \author Satofumi KAMIMURA

  $Id$

  \todo �G���f�B�A���̍��ق��z����������
  \todo GL �ł�����Ă΂��̂ŁAgetSdlColor() �͕ʃt�@�C���Ɉړ�������
*/

#include <SDL.h>


namespace beego {

  /*!
    \brief �F�̒�`
  */
  enum {
    Black =  0x000000ff,         //!< ��
    Blue =   0x0000ffff,         //!< ��
    Red =    0xff0000ff,         //!< ��
    Purple = 0xff00ffff,         //!< ��
    Green =  0x00ff00ff,         //!< ��
    Cyan =   0x00ffffff,         //!< ���F
    Yellow = 0xffff00ff,         //!< ���F
    White =  0xffffffff,         //!< ��

    Gray0 =  0x000000ff,
    Gray1 =  0x101010ff,
    Gray2 =  0x202020ff,
    Gray3 =  0x303030ff,
    Gray4 =  0x404040ff,
    Gray5 =  0x505050ff,
    Gray6 =  0x606060ff,
    Gray7 =  0x707070ff,
    Gray8 =  0x808080ff,
    Gray9 =  0x909090ff,
    Gray10 = 0xa0a0a0ff,
    Gray11 = 0xb0b0b0ff,
    Gray12 = 0xc0c0c0ff,
    Gray13 = 0xd0d0d0ff,
    Gray14 = 0xe0e0e0ff,
    Gray15 = 0xf0f0f0ff,
  };

  extern Uint32 getSdlColor(const SDL_Surface* scr, Uint32 draw_color);
};

#endif /* !GUI_COLORS_H */
