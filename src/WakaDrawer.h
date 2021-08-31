#ifndef WAKA_DRAWER_H
#define WAKA_DRAWER_H

/*!
  \file
  \brief 和歌の描画

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <SDL.h>
#include <memory>
#include <vector>


/*!
  \brief 和歌の描画
*/
class WakaDrawer {
  WakaDrawer(void);
  WakaDrawer(const WakaDrawer& rhs);
  WakaDrawer& operator = (const WakaDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  /*!
    \brief タイピングモード指定のコンストラクタ

    \param mode [i] タイピングモード
  */
  explicit WakaDrawer(TypingMode mode);
  ~WakaDrawer(void);

  /*!
    \brief 描画する和歌の登録

    \param index [i] 描画する和歌の番号
  */
  void setWakaIndex(size_t index);

  /*!
    \brief 入力候補の配置

    \param text [i] かな表示の入力候補
    \param roman_sample [i] ローマ字入力候補
  */
  void setInputSample(const std::vector<std::vector<Uint16> >& text,
                      const std::vector<Uint16>& roman_sample);

  /*!
    \brief 入力候補の更新

    \param ku_index [i] 下の句の何番目まで入力したか
    \param roman_sample [i] ローマ字入力候補
    \param need_bs [i] BackSpace キー押下を促す場合に true
  */
  void updateInputSample(size_t ku_index,
                         const std::vector<Uint16>& roman_sample,
                         bool need_bs);

  /*!
    \brief 再描画
  */
  void draw(void);

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

  void placeLeftNum(size_t left_num);
  void removeLeftNum(void);

  // !!! 描画位置の指定メソッドは、必要に応じて追加する
};

#endif /*! WAKA_DRAWER_H */
