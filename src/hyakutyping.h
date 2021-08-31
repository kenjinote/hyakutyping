#ifndef HYAKUTYPING_H
#define HYAKUTYPING_H

/*!
  \file
  \brief 百人一首タイピング

  \author Satofumi KAMIMURA

  $Id$

  \todo OpenGL 版でキー入力が処理されないのを、なんとかする
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif


// 連絡先
#ifndef PACKAGE_BUG_REPORT
#define PACKAGE_BUG_REPORT "satofumi@sourceforge.jp" //!< バグ連絡先
#endif


// バージョン
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "X.X.X"         //!< バージョン情報
#endif
const char*const VersionText = "version " PACKAGE_VERSION; //!< バージョン情報

#ifndef MAJOR_VERSION
#define MAJOR_VERSION "X"
#endif
#ifndef MINOR_VERSION
#define MINOR_VERSION "X"
#endif
#ifndef MICRO_VERSION
#define MICRO_VERSION "X"
#endif


// タイトル
#ifndef PACKAGE_STRING
#define PACKAGE_STRING "hyakutyping X.X.X"
#endif


// ネットワーク
enum {
  ServerPort = 57577,          //!< ネットワーク対戦用の接続ポート番号
};

#endif /* !HYAKUTYPING_H */
