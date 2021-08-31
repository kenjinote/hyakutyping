/*!
  \file
  \brief �S�l���^�C�s���O

  \author Satofumi KAMIMURA

  $Id$

  \todo -W �t���ŃR���p�C�����ʉ߂���悤�ɂ���

  \todo �{�����[���́A�ǂ�����ł��ύX�ł���悤�ɂ���
  \todo �t���X�N���[���̃L�[�𔽉f
  \todo �A�j���[�V�����A�C�R���̃W���M�[���C������
  \todo �}�E�X�Ŗ߂����Ƃ��ɁA�L�[���삪�㏑������Ă��܂��̂��Ȃ�Ƃ�����
  \todo �I�����܂����H Yes/No �̍��ڂ�����H
  \todo ���j���[�̑J�ڒ���̃J�[�\���Ń��j���[���ڂɈړ�����̂��A�}������
  \todo TextProperty ���� LabelComponent �����N���X�����ׂ����H
  \todo Ctrl �������ƁA�\�ȃL�[���͂ɂ��Ă̐������\�������Ƃ��H
  \todo �l�b�g���[�N�ΐ�A�̎���
  \todo ���ڂ��ύX����閈�ɁA���ʉ���炷
  \todo �t���X�N���[���̂Ƃ��́A�}�E�X�̈ړ����Ȃ���ΉB��
  \todo lexical_cast �̗�O���E���ׂ�
  \todo �H�����̃��j���[���ڂɁA�H�����A�C�R����t������Ƃ�
*/

#include "hyakutyping.h"
#include "hyakutyping_bmp.h"
#include "icon_mask.h"
#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "VoicePlayer.h"
#include "FirstMenu.h"
#include "CreateSurfaceFromArray.h"
#include "SdlVideo.h"

using namespace beego;
using namespace SDL;
//using namespace GL;


static std::string getSaveDirectory(void) {

  std::string savedir = SaveDir;
  if (! savedir.compare(0, 2, "~/")) {
    // "~/" ����n�܂�ꍇ�A���ϐ� HOME �Œu��������
    const char* home = getenv("HOME");
    if (home != NULL) {
      savedir.replace(0, 1, home);
    }
  }

  return savedir;
}


/*!
  \brief ���C���֐�
*/
int main(int argc, char *argv[]) {

  // �����̏���
  bool music_mute = false;

  // !!! �l�ݒ��ǂݏo���悤�ɕύX����
  // !!! �l�ݒ�A�����ݒ�A�̏��ɕ]�����X�V����
  bool fullscreen = false;
  for (int i = 1; i < argc; ++i) {
    if ((! strcmp("-m", argv[i])) || (! strcmp("--mute", argv[i]))) {
      // �~���[�g�ݒ�
      music_mute = true;

    } else if ((! strcmp("-fs", argv[i])) ||
               (! strcmp("--fullscreen", argv[i]))) {
      // �X�N���[���̍ő剻
      fullscreen = true;
    }
  }

  // ��ʂ̏�����
  SdlVideo video;
  video.setCaption(PACKAGE_STRING);
  SDL_Surface* icon = createSurface(hyakutyping_bmp,
                                    hyakutyping_bmp_width,
                                    hyakutyping_bmp_height);
  video.setIcon(icon, icon_mask);
  video.setFullscreenMode(fullscreen);
  if (! video.show()) {
    printf("SdlVideo::show(): %s\n", video.what());
    exit(1);
  }

  // �f�[�^�̑��݃`�F�b�N
  std::string savedir = getSaveDirectory();
  CommonResources* common = CommonResources::getObject(savedir.c_str());
  if (! CommonResources::accessFontData()) {
    // �t�H���g�f�[�^���Ȃ���΁A�I������
    // !!! �\�Ȃ�A�t�H���g�f�[�^�����s�o�C�i���Ɋ܂߂���
    exit(1);
  }
  if (! CommonResources::accessMusicData()) {
    music_mute = true;
  }

  // GUI �����̏�����
  common->initializeGui();

  // ���y�����̏�����
  if (! music_mute) {
    common->initializeMusic();
  } else {
    // �r�ݏグ���s��Ȃ��悤�ɂ���
    common->voice_->setMute();
  }

  // ���j���[��ʂ̌Ăяo��
  FirstMenu menu;
  menu.run();

  return 0;
}
