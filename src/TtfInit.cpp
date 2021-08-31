/*!
  \file
  \brief SDL_ttf �̏�����

  \author Satofumi KAMIMURA

  $Id$
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "SdlInit.h"
#include "TtfInit.h"
#if !HAVE_CONFIG_H || HAVE_LIBSDL_TTF
#include <SDL_ttf.h>
#endif

using namespace beego;


/*!
  \brief TtfInit �̓����N���X

  \todo LogManager �ɑΉ�������
*/
struct TtfInit::pImpl : private SdlInit {
  static bool initialized;
};

bool TtfInit::pImpl::initialized = false;


TtfInit::TtfInit(void) : pimpl(new pImpl) {
  if (pimpl->initialized == false) {
#if !HAVE_CONFIG_H || HAVE_LIBSDL_TTF
    if ((! TTF_WasInit()) && (TTF_Init() < 0)) {
      throw;
    }
    atexit(TTF_Quit);
#else
    // !!! �G���[���b�Z�[�W�̏o�͕�������������
    fprintf(stderr, "SDL_ttf is not installed!\n");
#endif
    pimpl->initialized = true;
  }
}


TtfInit::~TtfInit(void) {
}
