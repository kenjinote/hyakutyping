#ifndef NET_INIT_H
#define NET_INIT_H

/*!
  \file
  \brief SDL_net ‚Ì‰Šú‰»

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL_net ‚Ì‰Šú‰»
  */
  class NetInit {
    NetInit(const NetInit& rhs);
    NetInit& operator = (const NetInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    NetInit(void);
    ~NetInit(void);
  };
};

#endif /* !NET_INIT_H */
