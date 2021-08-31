#ifndef RESULT_DRAWER_H
#define RESULT_DRAWER_H

/*!
  \file
  \brief タイピング結果の表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>


/*!
  \brief タイピング結果の描画
*/
class ResultDrawer {
  ResultDrawer(void);
  ResultDrawer(const ResultDrawer& rhs);
  ResultDrawer& operator = (const ResultDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief タイピングモード付きのコンストラクタ

    \brief mode [i] タイピングモード
  */
  explicit ResultDrawer(TypingMode mode);
  ~ResultDrawer(void);

  /*!
    \brief 結果計算と表示準備
  */
  void createResult(void);

  /*!
    \brief 結果の表示

    \param ticks [i] 表示開始からの時間 [msec]
  */
  void drawResult(size_t ticks);

  /*!
    \brief コンポーネントの配置
  */
  void placeComponents(void);

  /*!
    \brief コンポーネントの削除
  */
  void removeComponents(void);

  /*!
    \brief キー入力の判定

    \retval true 決定キーが押下された
    \retval false 決定キーの押下なし
  */
  bool keyPressed(void);
};

#endif /* !RESULT_DRAWER_H */
