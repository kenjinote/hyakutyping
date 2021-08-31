#ifndef SHUFFLE_WAKA_H
#define SHUFFLE_WAKA_H

/*!
  \file
  \brief �a�̕��т̃V���b�t���N���X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>
#include <vector>


/*!
  \brief �a�̕��т̃V���b�t��
*/
class ShuffleWaka {
  ShuffleWaka(void);
  ShuffleWaka(const ShuffleWaka& rhs);
  ShuffleWaka& operator = (const ShuffleWaka& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  enum {
    NoShuffleSeed = -1,
  };

  /*!
    \brief �����_���V�[�h�w��̃R���X�g���N�^

    \param mode [i] �^�C�s���O���[�h
    \param seed [i] �����_���V�[�h

    \attention seed �� NoShuffleSeed ���w�肷��Ƙa�̂̕��т���ꊷ���Ȃ�
  */
  ShuffleWaka(TypingMode mode, int seed);
  ~ShuffleWaka(void);

  /*!
    \brief �a�̂̋����ړ�

    no �Ŏw�肳�ꂽ�a�̂��Arange_to �܂ł̈ʒu�ɋ����I�Ɉړ�������

    \param no [i] �a�̔ԍ�
    \param range_to [i] �z�u����͈�

    \attention shuffle() �O�ɌĂяo������
  */
  void setSomeWaka(int no, int range_to);

  /*!
    \brief �a�̂̃V���b�t��
  */
  void shuffle(std::vector<int>& waka);

  /*!
    \brief ���܂莚�̃O���[�v�����擾

    \param no [i] �a�̂̔ԍ�
  */
  std::vector<int> getKimarijiGroup(int no);
};

#endif /* !SHUFFLE_WAKA_H */
