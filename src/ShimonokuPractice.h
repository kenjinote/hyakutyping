#ifndef SHIMONOKU_PRACTICE_H
#define SHIMONOKU_PRACTICE_H

/*!
  \file
  \brief ���̋�̓��͗��K

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �^�C�s���O���K�̏�ԊǗ�
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
    \brief ���̋�^�C�s���O�̊J�n
  */
  void run(void);
};

#endif /* !SHIMONOKU_PRACTICE_H */
