#ifndef TYPING_TEXT_CONVERTER_H
#define TYPING_TEXT_CONVERTER_H

/*!
  \file
  \brief �^�C�s���O�p�̓��{��ϊ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TextConvertInterface.h"
#include <memory>

class TypingRecorder;

/*!
  \brief ���͕�����̕ϊ�
*/
class TypingTextConverter : public TextConvertInterface {
  TypingTextConverter(void);
  TypingTextConverter(const TypingTextConverter& rhs);
  TypingTextConverter& operator = (const TypingTextConverter& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:

  /*!
    \brief ���̓��[�h
  */
  typedef enum {
    Roman,                      //!< ���[�}������
    Kana,                       //!< ���ȓ���
  } ConvertType;

  /*!
    \brief ���̓��[�h�w��̃R���X�g���N�^

    \param type [i] ���̓��[�h
  */
  TypingTextConverter(ConvertType type);
  ~TypingTextConverter(void);

  void clear(void);
  void setConvertBuffer(const std::vector<Uint16>& text);
  void getConvertBuffer(std::vector<Uint16>& buffer);
  bool addChar(char ch);
  bool moveLeft(void);
  bool moveRight(void);
  bool moveUp(void);
  bool moveDown(void);
  bool escapePressed(void);
  bool deleteBack(void);
  bool deleteCurrent(void);
  bool convertInput(void);

  /*!
    \brief �L�^�I�u�W�F�N�g�̓o�^

    \param recorder [i] �o�^�p�̃I�u�W�F�N�g
  */
  void registerRecorder(TypingRecorder* recorder);

  /*!
    \brief �J�n�^�C�~���O�̓o�^

    \param start_ticks [i] �J�n�^�C�~���O
  */
  void registerStartTicks(size_t start_ticks);

  ConvertType getConvertType(void);
};

#endif /* !TYPING_TEXT_CONVERTER_H */
