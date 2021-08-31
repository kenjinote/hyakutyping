#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H

/*!
  \file
  \brief 履歴グラフの描画

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>


/*!
  \brief タイピング戦績のグラフ描画
*/
class GraphDrawer {
  GraphDrawer(void);
  GraphDrawer(const GraphDrawer& rhs);
  GraphDrawer& operator = (const GraphDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief コンストラクタ

    \param mode [i] 描画モード
  */
  explicit GraphDrawer(TypingMode mode);
  ~GraphDrawer(void);

  /*!
    \brief グラフ描画
  */
  bool drawGraph(void);

  /*!
    \brief コンポーネントの配置
  */
  void placeComponents(void);

  /*!
    \brief コンポーネントの除去
  */
  void removeComponents(void);

  /*!
    \brief 改行が押されたかを返す

    \retval true 押された
    \retval false 押されていない
  */
  bool keyPressed(void);
};

#endif /* !GRAPH_DRAWER_H */
