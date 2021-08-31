#ifndef PLAIN_TEXT_CONVERTER_H
#define PLAIN_TEXT_CONVERTER_H

/*!
  \file
  \brief –³•ÏŠ·ƒNƒ‰ƒX

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TextConvertInterface.h"
#include <memory>


namespace beego {
  class PlainTextConverter : public TextConvertInterface {
    PlainTextConverter(const PlainTextConverter& rhs);
    PlainTextConverter& operator = (const PlainTextConverter& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    PlainTextConverter(void);
    ~PlainTextConverter(void);

    void clear(void);
    void setConvertBuffer(const std::vector<Uint16>& buffer);
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
  };
};

#endif /* !PLAIN_TEXT_CONVERTER_H */
