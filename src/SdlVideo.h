#ifndef SDL_VIDEO_H
#define SDL_VIDEO_H

/*!
  \file
  \brief SDL ÇÃâÊñ ëÄçÏ

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <memory>


namespace beego {
  class SdlSurface;

  namespace SDL {
    /*!
      \brief SDL ÇÃâÊñ ëÄçÏÉNÉâÉX
    */
    class SdlVideo {
      SdlVideo(const SdlVideo& rhs);
      SdlVideo& operator = (const SdlVideo& rhs);

      struct pImpl;
      pImpl* pimpl;

    public:
      SdlVideo(void);
      ~SdlVideo(void);
      const char* what(void);

      void setFullscreenMode(bool on = true);
      bool isFullscreen(void);
      SDL_Surface* show(Uint32 mode = 0);
      void hide(void);

      void setCaption(const char* caption);
      void setIcon(SDL_Surface* icon, Uint8* mask = NULL);
      void setWindowBpp(size_t bpp);
      void setWindowSize(size_t width, size_t height);
      size_t getWidth(void);
      size_t getHeight(void);
    };
  };
};

#endif /* !SDL_VIDEO_H */
