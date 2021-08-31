#ifndef ACCESS_RECORD_INTERFACE_H
#define ACCESS_RECORD_INTERFACE_H

/*!
  \file
  \brief 記録データの操作インターフェース

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingRecorder.h"
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <time.h>


/*!
  \brief タイピング戦績の記録インターフェース
*/
class AccessRecordInterface {
public:
  virtual ~AccessRecordInterface(void) {}

  /*!
    \brief 記録

    \param data [i] 記録データ

    \retval true 正常
    \retval false エラー
  */
  virtual bool save(const TypingRecorder::GameSettings& data) = 0;

  /*!
    \brief 読み込み

    \param data [o] 読み出しデータ
    \param user_name [i] ユーザ名
    \param from [i] 読み出し範囲の開始位置
    \param to [i] 読み出し範囲の終了位置
    \param max_num [i] 読み出す最大データ数

    \retval true 正常
    \retval false エラー
  */
  virtual bool load(std::vector<TypingRecorder::GameSettings>& data,
                    const char* user_name,
                    time_t from, time_t to, size_t max_num) = 0;
};

#endif /* !ACCESS_RECORD_INTERFACE_H */
