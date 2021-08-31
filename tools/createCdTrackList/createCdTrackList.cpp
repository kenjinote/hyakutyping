/*!
  \file
  \brief CD ���̎擾

  \author Satofumi KAMIMURA

  $Id$

  \todo �������n����Ȃ������ꍇ�A�f�t�H���g�̃t�@�C���ɏ����o���H
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
