#ifndef GRAPH_DRAWER_UNI_H
#define GRAPH_DRAWER_UNI_H

/*!
  \file
  \brief グラフ表示用の Unicode リソース定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>

const static Uint16 GraphDrawer_title[] = { 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x5c65, 0x6b74, 0x0 }; // タイピング履歴
const static Uint16 GraphDrawer_shimonoku[] = { 0x4e0b, 0x306e, 0x53e5, 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x0 }; // 下の句タイピング
const static Uint16 GraphDrawer_kimariji[] = { 0x6c7a, 0x307e, 0x308a, 0x5b57, 0x30bf, 0x30a4, 0x30d4, 0x30f3, 0x30b0, 0x0 }; // 決まり字タイピング

const static Uint16 GraphDrawer_miss[] = { 0x8a02, 0x6b63, 0x30bf, 0x30a4, 0x30d7, 0x6570, ' ', '[', 0x56de, ']', 0x0 }; // 訂正タイプ数 [回]
const static Uint16 GraphDrawer_delay[] = { 0x5165, 0x529b, 0x958b, 0x59cb, 0x9045, 0x5ef6, ' ', '[', 0x79d2, ']', 0x0 }; // 入力開始遅延 [秒]
const static Uint16 GraphDrawer_speed[] = { 0x5165, 0x529b, 0x901f, 0x5ea6, ' ', '[', 0x56de, '/', 0x79d2, ']', 0x0 }; // 入力速度 [回/秒]

#endif /* !GRAPH_DRAWER_UNI_H */
