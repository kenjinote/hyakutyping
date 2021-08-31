#ifndef SDL_AUDIO_INIT_H
#define SDL_AUDIO_INIT_H

/*!
  \file
  \brief SDL 音楽資源の初期化

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief SDL 音楽資源の初期化クラス
  */
  class SdlAudioInit {
    SdlAudioInit(const SdlAudioInit& rhs);
    SdlAudioInit& operator = (const SdlAudioInit& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  protected:
    SdlAudioInit(void);
    ~SdlAudioInit(void);

    bool isInitialized(void);
  };
};

#endif /* !SDL_AUDIO_INIT_H */
