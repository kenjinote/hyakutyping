#ifndef CDROM_PLAYER_H
#define CDROM_PLAYER_H

/*!
  \file
  \brief CD-ROM の再生

  \author Satofumi KAMIMURA

  $Id$
*/

#include "SdlCdromInit.h"
#include <vector>

namespace beego {

  class CdromPlayer {
    CdromPlayer(const CdromPlayer& rhs);
    CdromPlayer& operator = (const CdromPlayer& rhs);

    struct pImpl;
    pImpl* pimpl;

  public:
    typedef enum {
      AudioTrack,
      DataTrack,
    } TrackType;

    typedef struct {
      int id;
      TrackType type;
      int frame_length;
      int min;
      int sec;
    } track_t;

    CdromPlayer(void);
    ~CdromPlayer(void);

    const char* what(void);

    bool getTrackList(std::vector<track_t>& list);
    size_t play(size_t id, size_t start_frame = 0, size_t play_frame = 0);
    void stop(void);

    // !!! 再生ドライブの指定
    // !!! CD の有無を確認？
    void eject(void);
    // !!! 再生中かを返す
    // !!! 一時停止
  };
};

#endif /* !CDROM_PLAYER_H */
