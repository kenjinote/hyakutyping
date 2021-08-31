#ifndef CDROM_PLAYER_H
#define CDROM_PLAYER_H

/*!
  \file
  \brief CD-ROM �̍Đ�

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

    // !!! �Đ��h���C�u�̎w��
    // !!! CD �̗L�����m�F�H
    void eject(void);
    // !!! �Đ�������Ԃ�
    // !!! �ꎞ��~
  };
};

#endif /* !CDROM_PLAYER_H */
