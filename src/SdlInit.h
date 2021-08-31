#ifndef SDL_INIT_H
#define SDL_INIT_H

/*!
  \file
  \brief SDL の初期化クラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL の初期化クラス
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
