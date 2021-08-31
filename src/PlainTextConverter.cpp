/*!
  \file
  \brief –³•ÏŠ·ƒNƒ‰ƒX

  \author Satofumi KAMIMURA

  $Id$
*/

#include "PlainTextConverter.h"

using namespace beego;


struct PlainTextConverter::pImpl {
  std::vector<Uint16> inputed_text;
};


PlainTextConverter::PlainTextConverter(void) : pimpl(new pImpl) {
}


PlainTextConverter::~PlainTextConverter(void) {
}


void PlainTextConverter::clear(void) {
  pimpl->inputed_text.clear();
}


void PlainTextConverter::setConvertBuffer(const std::vector<Uint16>& buffer) {
  pimpl->inputed_text = buffer;
}


void PlainTextConverter::getConvertBuffer(std::vector<Uint16>& buffer) {
  buffer = pimpl->inputed_text;
}


bool PlainTextConverter::addChar(char ch) {
  pimpl->inputed_text.push_back(ch);
  return true;
}


bool PlainTextConverter::moveLeft(void) {
  // !!!
  return false;
}


bool PlainTextConverter::moveRight(void) {
  // !!!
  return false;
}


bool PlainTextConverter::moveUp(void) {
  // !!!
  return false;
}


bool PlainTextConverter::moveDown(void) {
  // !!!
  return false;
}


bool PlainTextConverter::escapePressed(void) {
  // !!!
  return false;
}


bool PlainTextConverter::deleteBack(void) {
  if (pimpl->inputed_text.empty()) {
    return false;
  }

  pimpl->inputed_text.pop_back();
  return true;
}


bool PlainTextConverter::deleteCurrent(void) {
  // !!!
  return false;
}


bool PlainTextConverter::convertInput(void) {
  // !!!
  return false;
}
