#ifndef USER_MENU_UNI_H
#define USER_MENU_UNI_H

/*!
  \file
  \brief ユーザメニューの Unicode リソース定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>

const static Uint16 UserMenu_title[] = { 0x30e6, 0x30fc, 0x30b6, 0x7ba1, 0x7406, 0x0 }; // ユーザ管理
const static Uint16 UserMenu_add[] = { 0x65b0, 0x898f, 0x30e6, 0x30fc, 0x30b6, 0x306e, 0x4f5c, 0x6210, 0x0 }; // 新規ユーザの作成
const static Uint16 UserMenu_delete[] = { 0x30e6, 0x30fc, 0x30b6, 0x306e, 0x524a, 0x9664, 0x0 }; // ユーザの削除
const static Uint16 UserMenu_back[] = { 0x623b, 0x308b, 0x0 }; // 戻る

#endif /* ! USER_MENU_UNI_H */
