#ifndef SDL_INIT_H
#define SDL_INIT_H

/*!
  \file
  \brief SDL �̏������N���X

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL �̏������N���X
  */
  class SdlInit {
    SdlInit(const SdlInit& rhs);
    SdlInit& operator = (const SdlInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    SdlInit(void);
    ~SdlInit(void);
  };
};

#endif /* ! SDL_INIT_H */
