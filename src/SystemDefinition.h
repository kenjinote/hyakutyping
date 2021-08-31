#ifndef SYSTEM_DEFINITION_H
#define SYSTEM_DEFINITION_H

/*!
  \file
  \brief 進行についてのシステム定義

  \author Satofumi KAMIMURA

  $Id$
*/


// デフォルト設定
enum {
  DefaultPracticeNum = 5,
  GraphHistoryMax = 30,
};


// システム設定
enum {
  UserMax = 5,
};

// ゲーム設定
enum {
#if 1
  KaminokuPlaying = 6500,
  ShimonokuPlaying = 6000,
  YomibitoPlaying = 1000,
#else
  // デバッグ時用
  KaminokuPlaying = 45,
  ShimonokuPlaying = 30,
  YomibitoPlaying = 10,
#endif
  TotalPlaying = KaminokuPlaying + ShimonokuPlaying,
};

// Web アクセスのタイムアウト時間
enum {
  WebAccessTimeout = 3000,
};

// 効果の待機時間
enum {
  PrologueMsec = 5100,          //!< プロローグ時間の待機時間 [msec]
};

// 音楽の再生まわり
enum {
  PlayFadeIn = 1000,
  StopFadeOut = 200,
};

#endif /* !SYSTEM_DEFINITION_H */
