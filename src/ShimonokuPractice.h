#ifndef SHIMONOKU_PRACTICE_H
#define SHIMONOKU_PRACTICE_H

/*!
  \file
  \brief 下の句の入力練習

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief タイピング練習の状態管理
*/
class ShimonokuPractice {
  ShimonokuPractice(const ShimonokuPractice& rhs);
  ShimonokuPractice& operator = (const ShimonokuPractice& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  ShimonokuPractice(void);
  ~ShimonokuPractice(void);

  /*!
    \brief 下の句タイピングの開始
  */
  void run(void);
};

#endif /* !SHIMONOKU_PRACTICE_H */
