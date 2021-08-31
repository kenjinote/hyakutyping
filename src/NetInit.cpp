/*!
  \file
  \brief SDL_net �̏�����

  \author Satofumi KAMIMURA

  $Id$
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "SdlInit.h"
#include "NetInit.h"
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
#include <SDL_net.h>
#endif

using namespace beego;


/*!
  \brief NetInit �̓����N���X
*/
struct NetInit::pImpl : private SdlInit {
  static bool initialized;
};

bool NetInit::pImpl::initialized = false;


NetInit::NetInit(void) : pimpl(new pImpl) {
  if (pimpl->initialized == false) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_NET
    if (SDLNet_Init() < 0) {
      throw;
    }
    //printf("SDLNet_Init is called\n");
    atexit(SDLNet_Quit);
#else
    // !!! �G���[���b�Z�[�W�̏o�͕�������������
    fprintf(stderr, "SDL_net is not installed!\n");
#endif
    pimpl->initialized = true;
  }
}


NetInit::~NetInit(void) {
}
