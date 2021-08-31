#ifndef POSITION_DEFINITION_H
#define POSITION_DEFINITION_H

/*!
  \file
  \brief コンポーネントまわりの定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include "GuiColors.h"


// フォント色
enum {
  Fore = beego::Black,
  Back = beego::Gray12,
  Selected = beego::Blue,

  Pink = 0xffc0cbff,
  CornSilk = 0xfff8dcff,
  DarkGreen = 0x00a000ff,
};

// フォントサイズ
enum {
  TitleSize = 46,
  MenuSize = 30,

  LargeSize = 38,
  NormalSize = 24,
  SmallSize = 16,

  WakaDrawSize = 39,
  RubiDrawSize = 18,
  YomibitoDrawSize = 33,
  YomibitoRubiDrawSize = 16,
  RomanSize = 32,

  InputSize = 46,
};


// コンポーネントの配置まわり
enum {
  InputWidth = 400,
  ResultWidth = 420,
  TextOffset = 8,

  IconOffset = -48,

  MenuOffset = 70,
  ResultOffset = 50,
  SubOffset = 45,
  BottomOffset = 48,
  SampleOffset = 38,
  RomanOffset = 30,

  GraphWidth = 540,
  GraphHeight = 360,
};

#endif /* !POSITION_DEFINITION_H */
