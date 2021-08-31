#ifndef SDL_CDROM_INIT_H
#define SDL_CDROM_INIT_H

/*!
  \file
  \brief SDL CDROM �̏�����

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL CDROM �̏������N���X
  */
  class SdlCdromInit {
    SdlCdromInit(const SdlCdromInit& rhs);
    SdlCdromInit& operator = (const SdlCdromInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    SdlCdromInit(void);
    ~SdlCdromInit(void);

    bool isInitialized(void);
  };
};

#endif /* !SDL_CDROM_INIT_H */
