#ifndef PRACTICE_NUM_UNI_H
#define PRACTICE_NUM_UNI_H

/*!
  \file
  \brief �a�̐��� Unicode ���\�[�X��`

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>

const static Uint16 PracticeNum_5[] = { '5', 0x9996, 0x0 }; // 5��
const static Uint16 PracticeNum_10[] = { '1', '0', 0x9996, 0x0 }; // 10��
const static Uint16 PracticeNum_50[] = { '5', '0', 0x9996, 0x0 }; // 50��
const static Uint16 PracticeNum_100[] = { '1', '0', '0', 0x9996, 0x0 }; // 100��

const static Uint16 PracticeNum_left[] = { 0x6b8b, 0x308a, ' ', 0x0 }; // �c�� 
const static Uint16 PracticeNum_syu[] = { 0x9996, 0x0 }; // ��
const static Uint16 PracticeNum_complete[] = { 0x5165, 0x529b, 0x5b8c, 0x4e86, 0x0 }; // ���͊���

#endif /* ! PRACTICE_NUM_UNI_H */
