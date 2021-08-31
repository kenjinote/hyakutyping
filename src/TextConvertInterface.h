#ifndef TEXT_CONVERT_INTERFACE_H
#define TEXT_CONVERT_INTERFACE_H

/*!
  \file
  \brief �e�L�X�g�ϊ��̃C���^�[�t�F�[�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


/*!
  \brief ���͕ϊ��̃C���^�[�t�F�[�X
*/
class TextConvertInterface {
public:
  virtual ~TextConvertInterface(void) {}

  /*!
    \brief �ϊ��o�b�t�@�̃N���A
  */
  virtual void clear(void) = 0;

  /*!
    \brief �ϊ��o�b�t�@�ւ̓o�^

    \param text [i] �ϊ����镶����
  */
  virtual void setConvertBuffer(const std::vector<Uint16>& text) = 0;

  /*!
    \brief �ϊ��o�b�t�@�̎擾

    \param buffer [o] �o�b�t�@���̃f�[�^
  */
  virtual void getConvertBuffer(std::vector<Uint16>& buffer) = 0;

  /*!
    \brief �����̒ǉ�

    \param ch [i] ���͕���
  */
  virtual bool addChar(char ch) = 0;

  /*!
    \brief �J�[�\����
  */
  virtual bool moveLeft(void) = 0;

  /*!
    \brief �J�[�\���E
  */
  virtual bool moveRight(void) = 0;

  /*!
    \brief �J�[�\����
  */
  virtual bool moveUp(void) = 0;

  /*!
    \brief �J�[�\����
  */
  virtual bool moveDown(void) = 0;

  /*!
    \brief �G�X�P�[�v
  */
  virtual bool escapePressed(void) = 0;

  /*!
    \brief BackSpace �L�[
  */
  virtual bool deleteBack(void) = 0;

  /*!
    \brief Delete �L�[
  */
  virtual bool deleteCurrent(void) = 0;

  /*!
    \brief �ϊ��L�[
  */
  virtual bool convertInput(void) = 0;

  // �ϊ����̕\�������邽�߂̏��擾
  // !!!
};

#endif /* !TEXT_CONVERT_INTERFACE_H */
