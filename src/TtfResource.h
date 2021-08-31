#ifndef TTF_RESOURCE_H
#define TTF_RESOURCE_H

/*!
  \file
  \brief TTF �����̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL_ttf.h>
#include <memory>


namespace beego {
  /*!
    \brief TTF ���\�[�X�Ǘ�
  */

  class TtfResource {
    TtfResource(const TtfResource& rhs);
    TtfResource& operator = (const TtfResource& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit TtfResource(const char* ttf_file);
    ~TtfResource(void);
    TTF_Font* getResource(size_t size);
  };
};

#endif /* !TTF_RESOURCE_H */
