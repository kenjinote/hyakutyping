/*!
  \file
  \brief CD-ROM の再生

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CdromPlayer.h"
#include "SdlCdromInit.h"
#include <SDL.h>
#include <string>

using namespace beego;


struct CdromPlayer::pImpl : private SdlCdromInit {
  std::string error_message_;
  SDL_CD* cdrom_;
  size_t drives_;
  std::vector<track_t> track_list_;

  pImpl(void) : error_message_("no error."), cdrom_(NULL), drives_(0) {
    openDrive();
  }

  ~pImpl(void) {
    if (cdrom_) {
      stop();
      SDL_CDClose(cdrom_);
    }
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }

  // !!! ドライバ番号を引数に与えられるように変更する
  bool openDrive(void) {
    drives_ = SDL_CDNumDrives();
    //fprintf(stderr, "drives: %d\n", drives_);
    if (drives_ == 0) {
      error_message_ = "not found CD-ROM drive.";
      return false;
    }

    cdrom_ = SDL_CDOpen(0);
    if (! cdrom_) {
      error_message_ = SDL_GetError();
      return false;
    }

    return true;
  }

  bool checkDrive(void) {

    if (! cdrom_) {
      if (! openDrive()) {
        return false;
      }
    }

    CDstatus status = SDL_CDStatus(cdrom_);
    if (! CD_INDRIVE(status)) {
      error_message_ = "not found CD-ROM.";
      return false;
    }

    track_list_.clear();
    for (int i = 0; i < cdrom_->numtracks; ++i) {

      track_t info;
      SDL_CDtrack& track = cdrom_->track[i];
      info.id = track.id;
      info.frame_length = track.length;
      info.type = (track.type == SDL_AUDIO_TRACK) ? AudioTrack : DataTrack;

      int frame;
      FRAMES_TO_MSF(track.length, &info.min, &info.sec, &frame);

      track_list_.push_back(info);
    }

    return true;
  }

  bool getTrackList(std::vector<track_t>& list) {

    if (! checkDrive()) {
      return false;
    }

    list = track_list_;
    return true;
  }

  bool play(size_t id, size_t start_frame, size_t play_frame) {
    if (! checkDrive()) {
      return false;
    }

    if ((id >= track_list_.size()) || (track_list_[id].type == DataTrack)) {
      return false;
    }

    int ntrack = (play_frame == 0) ? 1 : 0;
    SDL_CDPlayTracks(cdrom_, static_cast<int>(id),
                     static_cast<int>(start_frame),
                     ntrack, static_cast<int>(play_frame));
    return true;
  }

  void stop(void) {
    SDL_CDStop(cdrom_);
  }
};


CdromPlayer::CdromPlayer(void) : pimpl(pImpl::getObject()) {
}


CdromPlayer::~CdromPlayer(void) {
}


const char* CdromPlayer::what(void) {
  return pimpl->error_message_.c_str();
}


bool CdromPlayer::getTrackList(std::vector<track_t>& list) {
  return pimpl->getTrackList(list);
}


size_t CdromPlayer::play(size_t id, size_t start_frame, size_t play_frame) {
  if (! pimpl->play(id, start_frame, play_frame)) {
    return 0;
  }

  size_t play_msec = 0;
  if (play_frame != 0) {
    play_msec = 1000 * play_frame / CD_FPS;

  } else {
    play_msec =
      1000 * (pimpl->track_list_[id].frame_length - start_frame) / CD_FPS;
  }
  return play_msec;
}


void CdromPlayer::stop(void) {
  pimpl->stop();
}


void CdromPlayer::eject(void) {
  SDL_CDEject(pimpl->cdrom_);
}
