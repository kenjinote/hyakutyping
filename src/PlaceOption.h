#ifndef PLACE_OPTION_H
#define PLACE_OPTION_H

/*!
  \file
  \brief �z�u�I�v�V�����̒�`

  \author Satofumi KAMIMURA

  \todo ������Atypedef enum �ɂ���A���H
*/

namespace beego {
  enum {
      None = 0x00,              //!< �w��Ȃ�
      Top = 0x01,               //!< ��
      Middle = 0x02,            //!< ��
      Bottom = 0x04,            //!< ��
      Left = 0x08,              //!< ��
      Center = 0x10,            //!< ����
      Right = 0x20,             //!< �E
  };
};

#endif /* !PLACE_OPTION_H */
