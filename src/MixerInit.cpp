/*!
  \file
  \brief SDL_mixer �̏������N���X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "MixerInit.h"
#include "SdlAudioInit.h"
#include "LogManager.h"
#include <SDL_mixer.h>
#include <stdlib.h>
#include <string>

using namespace beego;


/*!
  \brief MixerInit �̓����N���X

  \todo Mix �̎g���������Ȃ����������
*/
struct MixerInit::pImpl : private SdlAudioInit {
  static bool initialized;
  static void cleanup(void) {
    Mix_AllocateChannels(0);
  }

  pImpl(void) {
    if (! isInitialized()) {
      return;
    }

    if (initialized == false)  {
      if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                        MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        // ���O�L�^
        LogManager* log = LogManager::getObject();
        std::string message = "Mixer_Init(): " + std::string(Mix_GetError());
        log->write(LogManager::Error, message.c_str());
        return;
      }
      atexit(Mix_CloseAudio);

      Mix_AllocateChannels(ChannelNum);
      atexit(cleanup);
      initialized = true;
    }
  }
};

bool MixerInit::pImpl::initialized = false;


MixerInit::MixerInit(void) : pimpl(new pImpl) {
}


MixerInit::~MixerInit(void) {
}


bool MixerInit::isInitialized(void) {
  return pimpl->initialized;
}
