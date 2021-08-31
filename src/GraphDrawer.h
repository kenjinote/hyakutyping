#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H

/*!
  \file
  \brief �����O���t�̕`��

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>


/*!
  \brief �^�C�s���O��т̃O���t�`��
*/
class GraphDrawer {
  GraphDrawer(void);
  GraphDrawer(const GraphDrawer& rhs);
  GraphDrawer& operator = (const GraphDrawer& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief �R���X�g���N�^

    \param mode [i] �`�惂�[�h
  */
  explicit GraphDrawer(TypingMode mode);
  ~GraphDrawer(void);

  /*!
    \brief �O���t�`��
  */
  bool drawGraph(void);

  /*!
    \brief �R���|�[�l���g�̔z�u
  */
  void placeComponents(void);

  /*!
    \brief �R���|�[�l���g�̏���
  */
  void removeComponents(void);

  /*!
    \brief ���s�������ꂽ����Ԃ�

    \retval true �����ꂽ
    \retval false ������Ă��Ȃ�
  */
  bool keyPressed(void);
};

#endif /* !GRAPH_DRAWER_H */
