#ifndef SDL_VIDEO_INIT_H
#define SDL_VIDEO_INIT_H

/*!
  \file
  \brief SDL_VIDEO �̏������N���X

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL �̏������N���X
  */
  class SdlVideoInit {
    SdlVideoInit(const SdlVideoInit& rhs);
    SdlVideoInit& operator = (const SdlVideoInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    SdlVideoInit(void);
    ~SdlVideoInit(void);
  };
};

#endif /* ! SDL_VIDEO_INIT_H */
