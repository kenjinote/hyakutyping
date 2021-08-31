#ifndef RESULT_DRAWER_UNI_H
#define RESULT_DRAWER_UNI_H

/*!
  \file
  \brief ���ʕ\���p�� Unicode ���\�[�X��`

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>

const static Uint16 ResultDrawer_title[] = { 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x7d50, 0x679c, 0x0 }; // �^�C�s���O����
const static Uint16 ResultDrawer_num[] = { 0x548c, 0x6b4c, 0x6570, 0x0 }; // �a�̐�
const static Uint16 ResultDrawer_syu[] = { ' ', '[', 0x9996, ']', 0x0 }; //  [��]
const static Uint16 ResultDrawer_types[] = { 0x30bf, 0x30a4, 0x30d7, 0x901f, 0x5ea6, 0x0 }; // �^�C�v���x
const static Uint16 ResultDrawer_vel[] = { ' ', '[', 0x56de, '/', 0x79d2, ']', 0x0 }; //  [��/�b]
const static Uint16 ResultDrawer_delay[] = { 0x5165, 0x529b, 0x958b, 0x59cb, 0x9045, 0x5ef6, 0x0 }; // ���͊J�n�x��
const static Uint16 ResultDrawer_sec[] = { ' ', '[', 0x79d2, ']', 0x0 }; //  [�b]
const static Uint16 ResultDrawer_miss[] = { 0x8a02, 0x6b63, 0x30bf, 0x30a4, 0x30d7, 0x6570, 0x0 }; // �����^�C�v��
const static Uint16 ResultDrawer_times[] = { ' ', '[', 0x56de, ']', 0x0 }; //  [��]

#endif /* !RESULT_DRAWER_UNI_H */
