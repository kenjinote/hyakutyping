#ifndef WAKA_DRAWER_H
#define WAKA_DRAWER_H

/*!
  \file
  \brief �a�̂̕`��

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <SDL.h>
#include <memory>
#include <vector>


/*!
  \brief �a�̂̕`��
*/
class WakaDrawer {
  WakaDrawer(void);
  WakaDrawer(const WakaDrawer& rhs);
  WakaDrawer& operator = (const WakaDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  /*!
    \brief �^�C�s���O���[�h�w��̃R���X�g���N�^

    \param mode [i] �^�C�s���O���[�h
  */
  explicit WakaDrawer(TypingMode mode);
  ~WakaDrawer(void);

  /*!
    \brief �`�悷��a�̂̓o�^

    \param index [i] �`�悷��a�̂̔ԍ�
  */
  void setWakaIndex(size_t index);

  /*!
    \brief ���͌��̔z�u

    \param text [i] ���ȕ\���̓��͌��
    \param roman_sample [i] ���[�}�����͌��
  */
  void setInputSample(const std::vector<std::vector<Uint16> >& text,
                      const std::vector<Uint16>& roman_sample);

  /*!
    \brief ���͌��̍X�V

    \param ku_index [i] ���̋�̉��Ԗڂ܂œ��͂�����
    \param roman_sample [i] ���[�}�����͌��
    \param need_bs [i] BackSpace �L�[�����𑣂��ꍇ�� true
  */
  void updateInputSample(size_t ku_index,
                         const std::vector<Uint16>& roman_sample,
                         bool need_bs);

  /*!
    \brief �ĕ`��
  */
  void draw(void);

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

  void placeLeftNum(size_t left_num);
  void removeLeftNum(void);

  // !!! �`��ʒu�̎w�胁�\�b�h�́A�K�v�ɉ����Ēǉ�����
};

#endif /*! WAKA_DRAWER_H */
