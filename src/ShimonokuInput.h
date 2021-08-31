#ifndef SHIMONOKU_INPUT_H
#define SHIMONOKU_INPUT_H

/*!
  \file
  \brief 下の句の入力処理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include "TypingTextConverter.h"


/*!
  \brief 下の句の入力
*/
class ShimonokuInput {
  ShimonokuInput(void);
  ShimonokuInput(const ShimonokuInput& rhs);
  ShimonokuInput& operator = (const ShimonokuInput& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief モード指定のオブジェクト生成

    \param mode [i] タイピングモード
  */
  explicit ShimonokuInput(TypingMode mode,
                          TypingTextConverter::ConvertType type);
  ~ShimonokuInput(void);

  /*!
    \brief 和歌表示の準備
  */
  void updateWaka(void);

  /*!
    \brief 次の和歌に更新
  */
  void setNextWaka(void);

  size_t getCurrentWakaNo(void);

  /*!
    \brief 句の表示処理
  */
  void drawWaka(void);

  /*!
    \brief 入力と例題の比較
  */
  void drawInput(void);

  /*!
    \brief 和歌の入力が完了したか
  */
  bool isComplete(void);

  /*!
    \brief コンポーネントの配置
  */
  void placeComponents(void);

  /*!
    \brief コンポーネントの削除
  */
  void removeComponents(void);

  /*!
    \brief 入力コンポーネントの削除
  */
  void removeInputComponents(void);

  /*!
    \brief 入力文字があるかどうか

    \retval true 入力文字あり
    \retval false 入力文字なし
  */
  bool textEmpty(void);

  void placeLeftNum(size_t left_num);
  void removeLeftNum(void);
};

#endif /* !SHIMONOKU_INPUT_H */
