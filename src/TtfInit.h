#ifndef TTF_INIT_H
#define TTF_INIT_H

/*!
  \file
  \brief SDL_ttf ‚Ì‰Šú‰»

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL_ttf ‚Ì‰Šú‰»
  */
  class TtfInit {
    TtfInit(const TtfInit& rhs);
    TtfInit& operator = (const TtfInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    TtfInit(void);
    ~TtfInit(void);
  };
};

#endif /* !TTF_INIT_H */
