#ifndef TYPING_TEXT_CONVERTER_H
#define TYPING_TEXT_CONVERTER_H

/*!
  \file
  \brief タイピング用の日本語変換

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TextConvertInterface.h"
#include <memory>

class TypingRecorder;

/*!
  \brief 入力文字列の変換
*/
class TypingTextConverter : public TextConvertInterface {
  TypingTextConverter(void);
  TypingTextConverter(const TypingTextConverter& rhs);
  TypingTextConverter& operator = (const TypingTextConverter& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:

  /*!
    \brief 入力モード
  */
  typedef enum {
    Roman,                      //!< ローマ字入力
    Kana,                       //!< かな入力
  } ConvertType;

  /*!
    \brief 入力モード指定のコンストラクタ

    \param type [i] 入力モード
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
    \brief 記録オブジェクトの登録

    \param recorder [i] 登録用のオブジェクト
  */
  void registerRecorder(TypingRecorder* recorder);

  /*!
    \brief 開始タイミングの登録

    \param start_ticks [i] 開始タイミング
  */
  void registerStartTicks(size_t start_ticks);

  ConvertType getConvertType(void);
};

#endif /* !TYPING_TEXT_CONVERTER_H */
