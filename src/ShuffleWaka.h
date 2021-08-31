#ifndef SHUFFLE_WAKA_H
#define SHUFFLE_WAKA_H

/*!
  \file
  \brief 和歌並びのシャッフルクラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>
#include <vector>


/*!
  \brief 和歌並びのシャッフル
*/
class ShuffleWaka {
  ShuffleWaka(void);
  ShuffleWaka(const ShuffleWaka& rhs);
  ShuffleWaka& operator = (const ShuffleWaka& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  enum {
    NoShuffleSeed = -1,
  };

  /*!
    \brief ランダムシード指定のコンストラクタ

    \param mode [i] タイピングモード
    \param seed [i] ランダムシード

    \attention seed に NoShuffleSeed を指定すると和歌の並びを入れ換えない
  */
  ShuffleWaka(TypingMode mode, int seed);
  ~ShuffleWaka(void);

  /*!
    \brief 和歌の強制移動

    no で指定された和歌を、range_to までの位置に強制的に移動させる

    \param no [i] 和歌番号
    \param range_to [i] 配置する範囲

    \attention shuffle() 前に呼び出すこと
  */
  void setSomeWaka(int no, int range_to);

  /*!
    \brief 和歌のシャッフル
  */
  void shuffle(std::vector<int>& waka);

  /*!
    \brief 決まり字のグループ情報を取得

    \param no [i] 和歌の番号
  */
  std::vector<int> getKimarijiGroup(int no);
};

#endif /* !SHUFFLE_WAKA_H */
