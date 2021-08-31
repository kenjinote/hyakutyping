#ifndef HTTP_ACCESS_H
#define HTTP_ACCESS_H

/*!
  \file
  \brief HTTP アクセス

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <vector>


namespace beego {
  class HttpAccess {
    HttpAccess(void);
    HttpAccess(const HttpAccess& rhs);
    HttpAccess& operator = (const HttpAccess& rhs);

    struct pImpl;
    pImpl* pimpl;

  public:
    enum {
      DefaultPort = 80,
    };
    HttpAccess(const char* server, long port = DefaultPort);
    ~HttpAccess(void);
    const char* what(void);

    bool get(std::vector<char>& page, const char* address);
  };
};

#endif /* !HTTP_ACCESS_H */
