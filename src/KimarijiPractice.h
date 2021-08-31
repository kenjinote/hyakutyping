#ifndef KIMARIJI_PRACTICE_H
#define KIMARIJI_PRACTICE_H

/*!
  \file
  \brief 決まり字の入力練習

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief タイピング練習の状態管理
*/
class KimarijiPractice {
  KimarijiPractice(const KimarijiPractice& rhs);
  KimarijiPractice& operator = (const KimarijiPractice& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  KimarijiPractice(void);
  ~KimarijiPractice(void);

  /*!
    \brief 決まり字タイピングの開始
  */
  void run(void);
};

#endif /* !KIMARIJI_PRACTICE_H */
