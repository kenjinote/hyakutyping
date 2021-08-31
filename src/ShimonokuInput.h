#ifndef SHIMONOKU_INPUT_H
#define SHIMONOKU_INPUT_H

/*!
  \file
  \brief ���̋�̓��͏���

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include "TypingTextConverter.h"


/*!
  \brief ���̋�̓���
*/
class ShimonokuInput {
  ShimonokuInput(void);
  ShimonokuInput(const ShimonokuInput& rhs);
  ShimonokuInput& operator = (const ShimonokuInput& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief ���[�h�w��̃I�u�W�F�N�g����

    \param mode [i] �^�C�s���O���[�h
  */
  explicit ShimonokuInput(TypingMode mode,
                          TypingTextConverter::ConvertType type);
  ~ShimonokuInput(void);

  /*!
    \brief �a�̕\���̏���
  */
  void updateWaka(void);

  /*!
    \brief ���̘a�̂ɍX�V
  */
  void setNextWaka(void);

  size_t getCurrentWakaNo(void);

  /*!
    \brief ��̕\������
  */
  void drawWaka(void);

  /*!
    \brief ���͂Ɨ��̔�r
  */
  void drawInput(void);

  /*!
    \brief �a�̂̓��͂�����������
  */
  bool isComplete(void);

  /*!
    \brief �R���|�[�l���g�̔z�u
  */
  void placeComponents(void);

  /*!
    \brief �R���|�[�l���g�̍폜
  */
  void removeComponents(void);

  /*!
    \brief ���̓R���|�[�l���g�̍폜
  */
  void removeInputComponents(void);

  /*!
    \brief ���͕��������邩�ǂ���

    \retval true ���͕�������
    \retval false ���͕����Ȃ�
  */
  bool textEmpty(void);

  void placeLeftNum(size_t left_num);
  void removeLeftNum(void);
};

#endif /* !SHIMONOKU_INPUT_H */
