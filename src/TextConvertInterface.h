#ifndef TEXT_CONVERT_INTERFACE_H
#define TEXT_CONVERT_INTERFACE_H

/*!
  \file
  \brief テキスト変換のインターフェース

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <vector>


/*!
  \brief 入力変換のインターフェース
*/
class TextConvertInterface {
public:
  virtual ~TextConvertInterface(void) {}

  /*!
    \brief 変換バッファのクリア
  */
  virtual void clear(void) = 0;

  /*!
    \brief 変換バッファへの登録

    \param text [i] 変換する文字列
  */
  virtual void setConvertBuffer(const std::vector<Uint16>& text) = 0;

  /*!
    \brief 変換バッファの取得

    \param buffer [o] バッファ内のデータ
  */
  virtual void getConvertBuffer(std::vector<Uint16>& buffer) = 0;

  /*!
    \brief 文字の追加

    \param ch [i] 入力文字
  */
  virtual bool addChar(char ch) = 0;

  /*!
    \brief カーソル左
  */
  virtual bool moveLeft(void) = 0;

  /*!
    \brief カーソル右
  */
  virtual bool moveRight(void) = 0;

  /*!
    \brief カーソル上
  */
  virtual bool moveUp(void) = 0;

  /*!
    \brief カーソル下
  */
  virtual bool moveDown(void) = 0;

  /*!
    \brief エスケープ
  */
  virtual bool escapePressed(void) = 0;

  /*!
    \brief BackSpace キー
  */
  virtual bool deleteBack(void) = 0;

  /*!
    \brief Delete キー
  */
  virtual bool deleteCurrent(void) = 0;

  /*!
    \brief 変換キー
  */
  virtual bool convertInput(void) = 0;

  // 変換候補の表示をするための情報取得
  // !!!
};

#endif /* !TEXT_CONVERT_INTERFACE_H */
