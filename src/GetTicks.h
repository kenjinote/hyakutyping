#ifndef GET_TICKS_H
#define GET_TICKS_H

/*!
  \file
  \brief ���ԊԊu�̎擾

  \author Satofumi KAMIMURA

  $Id$

  \todo monitor �Ή��ɂ���
*/

namespace beego {
  /*!
    \brief �v���O�������쎞�Ԃ̎擾

    \return �v���O�������쎞�� [msec]

    \attention ���ۂɂ́ASDL_Init() ���Ă΂�Ă���̎��ԂƂȂ�B
  */
  extern size_t GetTicks(void);
};

#endif /* !GET_TICKS_H */
