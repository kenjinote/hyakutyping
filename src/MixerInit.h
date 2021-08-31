#ifndef MIXIER_INIT_H
#define MIXIER_INIT_H

/*!
  \file
  \brief SDL_mixer の初期化クラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL_mixer の初期化クラス
  */
  class MixerInit {
    MixerInit(const MixerInit& rhs);
    MixerInit& operator = (const MixerInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    MixerInit(void);
    ~MixerInit(void);

    bool isInitialized(void);

  public:
    enum {
      ChannelNum = 8,
    };
  };
};

#endif /* !MIXIER_INIT_H */
