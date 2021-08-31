#ifndef TYPING_RECORDER_H
#define TYPING_RECORDER_H

/*!
  \file
  \brief タイピング情報の記録

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>
#include <string>
#include <vector>


/*!
  \brief タイピング情報の記録クラス
*/
class TypingRecorder {
  TypingRecorder(const TypingRecorder& rhs);
  TypingRecorder& operator = (const TypingRecorder& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief 特殊入力
  */
  enum {
    Clear = 0x1,                //!< クリア入力
    BackSpace = 0x2,            //!< バックスペース入力
    Return = 0x3,               //!< 改行入力
    Complete = 0x4,             //!< 入力完了
  };

  /*!
    \brief キー入力情報
  */
  typedef struct {
    int ticks;                  //!< 入力タイミング
    char ch;                    //!< 入力キー
  } KeyTiming;

  /*!
    \brief 和歌情報
  */
  typedef struct {
    int no;                      //!< 和歌番号
    std::vector<KeyTiming> keys; //!< 入力キー情報
  } WakaTyping;

  /*!
    \brief 記録詳細の定義
  */
  typedef struct {
    int types;                    //!< タイプ数 [回]
    int miss_types;               //!< タイプミスの回数 [回]
    int delay_msec;               //!< 入力開始遅延 [msec]
    int elapse_msec;              //!< 経過時間 [msec]
  } ScoreData;

  /*!
    \brief ゲーム情報
  */
  typedef struct {
    std::string major_version;
    std::string minor_version;
    std::string micro_version;

    TypingMode mode;              //!< タイピングモード
    std::string user_name;        //!< ユーザ名
    time_t time;                  //!< 記録された時刻
    int rand_seed;                //!< ランダムシード
    ScoreData score;              //!< スコア情報
    int waka_num;                 //!< 和歌数
    std::vector<WakaTyping> waka; //!< 和歌情報
    int kaminoku_speed;           //!< 上の句の再生速度
    int shimonoku_speed;           //!< 下の句の再生速度
  } GameSettings;

  TypingRecorder(void);
  ~TypingRecorder(void);

  /*!
    \brief ゲーム情報の記録開始

    \param rand_seed [i] ランダムシード
    \param mode [i] タイピングモード
  */
  void recordGame(int rand_seed, TypingMode mode);

  /*!
    \brief 和歌情報の記録開始

    \param no [i] 和歌番号
  */
  void recordWaka(size_t no);

  /*!
    \brief タイピング情報の記録

    \param ch [i] 入力キー
    \param ticks [i] 入力タイピング
  */
  void recordTyping(char ch, size_t ticks);

  /*!
    \brief 登録内容の評価

    \param user_name [i] ユーザ名
  */
  void evaluate(const char* user_name);

  /*!
    \brief 和歌数の取得
  */
  size_t getWakaNum(void);

  /*!
    \brief 入力遅延の取得
  */
  double getStartDelay(void);

  /*!
    \brief タイピングスピードの取得
  */
  double getTypingSpeed(void);

  /*!
    \brief タイプミス数の取得
  */
  size_t getMissTypes(void);

  /*!
    \brief ゲームデータを返す
  */
  GameSettings& getRecordData(void) const;

  /*!
    \brief ゲームデータのエンコード
  */
  static std::string encodeGameData(const GameSettings& data);

  /*!
    \brief デバッグ表示用
  */
  void print(void);
};

#endif /* !TYPING_RECORDER_H */
