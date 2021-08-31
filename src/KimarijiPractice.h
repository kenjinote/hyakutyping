#ifndef KIMARIJI_PRACTICE_H
#define KIMARIJI_PRACTICE_H

/*!
  \file
  \brief ���܂莚�̓��͗��K

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


/*!
  \brief �^�C�s���O���K�̏�ԊǗ�
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
    \brief ���܂莚�^�C�s���O�̊J�n
  */
  void run(void);
};

#endif /* !KIMARIJI_PRACTICE_H */
