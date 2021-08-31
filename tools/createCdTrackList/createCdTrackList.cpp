/*!
  \file
  \brief CD 情報の取得

  \author Satofumi KAMIMURA

  $Id$

  \todo 引数が渡されなかった場合、デフォルトのファイルに書き出す？
*/

#include "CdromPlayer.h"
#include <SDL.h>

using namespace beego;


int main(int argc, char *argv[]) {
  static_cast<void>(argc);
  static_cast<void>(argv);

  CdromPlayer cdrom;

  std::vector<CdromPlayer::track_t> list;
  cdrom.getTrackList(list);

  for (std::vector<CdromPlayer::track_t>::iterator it = list.begin();
       it != list.end(); ++it) {
    printf("%d\n", static_cast<int>(it->frame_length));
  }

  return 0;
}
