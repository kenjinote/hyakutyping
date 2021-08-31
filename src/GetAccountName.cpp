/*!
  \file
  \brief ƒ†[ƒU–¼‚Ìæ“¾

  \author Satofumi KAMIMURA

  $Id$
*/

#include "GetAccountName.h"
#include "DetectOS.h"
#ifdef WINDOWS_OS
#include <windows.h>
#endif
#include <stdlib.h>


std::string beego::GetAccountName(void) {

#ifdef WINDOWS_OS
  enum { NameMax = 32 };
  DWORD length = NameMax;
  char buffer[NameMax];
  if (GetUserNameA(buffer, &length)) {
    return std::string(buffer);
  } else {
    return "";
  }
#else
  // Linux —pÀ‘•
  const char* p = getenv("USER");
  return std::string((p == NULL) ? "" : p);
#endif
}
