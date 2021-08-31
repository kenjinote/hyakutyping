#ifndef RESULT_DRAWER_H
#define RESULT_DRAWER_H

/*!
  \file
  \brief �^�C�s���O���ʂ̕\��

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>


/*!
  \brief �^�C�s���O���ʂ̕`��
*/
class ResultDrawer {
  ResultDrawer(void);
  ResultDrawer(const ResultDrawer& rhs);
  ResultDrawer& operator = (const ResultDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief �^�C�s���O���[�h�t���̃R���X�g���N�^

    \brief mode [i] �^�C�s���O���[�h
  */
  explicit ResultDrawer(TypingMode mode);
  ~ResultDrawer(void);

  /*!
    \brief ���ʌv�Z�ƕ\������
  */
  void createResult(void);

  /*!
    \brief ���ʂ̕\��

    \param ticks [i] �\���J�n����̎��� [msec]
  */
  void drawResult(size_t ticks);

  /*!
    \brief �R���|�[�l���g�̔z�u
  */
  void placeComponents(void);

  /*!
    \brief �R���|�[�l���g�̍폜
  */
  void removeComponents(void);

  /*!
    \brief �L�[���͂̔���

    \retval true ����L�[���������ꂽ
    \retval false ����L�[�̉����Ȃ�
  */
  bool keyPressed(void);
};

#endif /* !RESULT_DRAWER_H */
