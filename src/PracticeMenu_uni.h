#ifndef PRACTICE_MENU_UNI_H
#define PRACTICE_MENU_UNI_H

/*!
  \file
  \brief 練習メニューの Unicode リソース定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>


const static Uint16 PracticeMenu_title[] = { 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x7df4, 0x7fd2, 0x0 }; // タイピング練習
const static Uint16 PracticeMenu_shimonoku[] = { 0x4e0b, 0x306e, 0x53e5, 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x0 }; // 下の句タイピング
const static Uint16 PracticeMenu_kimariji[] = { 0x6c7a, 0x307e, 0x308a, 0x5b57, 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x0 }; // 決まり字タイピング
const static Uint16 PracticeMenu_back[] = { 0x3082, 0x3069, 0x308b, 0x0 }; // もどる

#endif /* ! PRACTICE_MENU_UNI_H */
