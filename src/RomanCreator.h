#ifndef ROMAN_CREATOR_H
#define ROMAN_CREATOR_H

/*!
  \file
  \brief ƒ[ƒ}š“ü—ÍŒó•â‚Ö‚Ì•ÏŠ·

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <memory>
#include <vector>


namespace beego {
  class RomanCreator {
    RomanCreator(const RomanCreator& rhs);
    RomanCreator& operator = (const RomanCreator& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    RomanCreator(void);
    ~RomanCreator(void);
    void convert(std::vector<Uint16>& dst, const Uint16* input);
  };
};

#endif /* !ROMAN_CREATOR_H */
