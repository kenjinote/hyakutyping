/*!
  \file
  \brief 指定トラックの再生サンプル

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CdromPlayer.h"
#include <SDL.h>

using namespace beego;


int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage:\n\t%s <track id> <first flame> <play flames>\n", argv[0]);
    exit(1);
  }

  int track_id = -1;
  if (argc >= 2) {
    track_id = atoi(argv[1]);
  }
  int first_frame = 0;
  if (argc >= 3) {
    first_frame = atoi(argv[2]);
  }
  int play_interval = 0;
  if (argc >= 4) {
    play_interval = atoi(argv[3]);
  }

  CdromPlayer cdrom;
  std::vector<CdromPlayer::track_t> tracks;
  if (! cdrom.getTrackList(tracks)) {
    printf("CdromPlayer::getTrackList: %s\n", cdrom.what());
    exit(1);
  }

  if (track_id >= 0) {
    printf("play: %d, %d, %d, ([%02d]-[%02d])\n",
           track_id, first_frame, play_interval,
           first_frame / CD_FPS, play_interval / CD_FPS);
    size_t msec = cdrom.play(track_id - 1, first_frame, play_interval);
    SDL_Delay(msec);
  }
  return 0;
}
