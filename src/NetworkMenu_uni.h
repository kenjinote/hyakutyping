#ifndef NETWORK_MENU_UNI_H
#define NETWORK_MENU_UNI_H

/*!
  \file
  \brief ネットワーク対戦メニューの Unicode リソース定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>

const static Uint16 NetworkMenu_title[] = { 0x30cd, 0x30c3, 0x30c8, 0x30ef, 0x30fc, 0x30af, 0x5bfe, 0x6226, '(', 0x307e, 0x3060, ')', 0x0 }; // ネットワーク対戦(まだ)
const static Uint16 NetworkMenu_player[] = { 0x30d7, 0x30ec, 0x30a4, 0x30e4, 0x30fc, 0x3068, 0x306e, 0x5bfe, 0x6226, '(', 0x307e, 0x3060, ')', 0x0 }; // プレイヤーとの対戦(まだ)
const static Uint16 NetworkMenu_data[] = { 0x904e, 0x53bb, 0x30c7, 0x30fc, 0x30bf, 0x3068, 0x306e, 0x5bfe, 0x6226, '(', 0x307e, 0x3060, ')', 0x0 }; // 過去データとの対戦(まだ)
const static Uint16 NetworkMenu_replay[] = { 0x5bfe, 0x6226, 0x30c7, 0x30fc, 0x30bf, 0x306e, 0x95b2, 0x89a7, '(', 0x307e, 0x3060, ')', 0x0 }; // 対戦データの閲覧(まだ)
const static Uint16 NetworkMenu_back[] = { 0x3082, 0x3069, 0x308b, 0x0 }; // もどる

#endif /* ! NETWORK_MENU_UNI_H */
