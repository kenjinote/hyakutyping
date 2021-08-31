/*!
  \file
  \brief �a�̂̉r�ݏグ

  \author Satofumi KAMIMURA

  $Id$

  \todo CD-ROM �̔F��������������A���̎|�����b�Z�[�W�ŏo�͂��ׂ�
  \todo �G���[�o�͂ɂ́Acommon_->debug ���ALogManager ���g��
  \todo �r�ݏグ�@�\�� On / Off �ɑΉ�������
*/

#include "VoicePlayer.h"
#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "CdromPlayer.h"
#include "FindFiles.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <fstream>

using namespace beego;
using namespace boost;
using namespace boost::xpressive;


struct VoicePlayer::pImpl {

  enum {
    InvalidTrack = -1,
  };

  CommonResources* common_;
  std::string play_resource_;
  bool mute_;                   //!< �r�ݏグ���s��Ȃ�

  int play_id_;                 //!< ���ʉ��̍Đ��`���l��
  std::vector<std::string> play_files_; //!< �Đ��f�[�^�̃t�@�C����

  /*!
    \brief �Đ����̊i�[�p
  */
  typedef struct {
    int track_id;               //!< �g���b�N�ԍ�
    size_t start_frame;         //!< �Đ��J�n�t���[���ʒu
    size_t play_interval;       //!< �Đ��t���[���Ԋu
  } waka_play_t;

  std::string dir_path_;
  CdromPlayer cdrom_;
  waka_play_t waka_play_[100];

  pImpl(const char* directory)
    : common_(NULL), play_resource_(""), mute_(false),
      play_id_(0), dir_path_(directory) {

    play_files_.resize(100);
  }

  bool readWakaInfo(const char* waka_file) {

    std::string path = std::string(dir_path_) + waka_file;
    //fprintf(stderr, "%s\n", path.c_str());

    // �a�̏��̏�����
    for (size_t i = 0; i < 100; ++i) {
      waka_play_[i].track_id = InvalidTrack;
    }

    // �e�a�̖��ɁA�ȉ��̓��e��ǂݏo��
    // - �g���b�N
    // - �J�n�t���[���ʒu
    // - �Đ��t���[���Ԋu
    std::ifstream fin(path.c_str());
    std::string line;
    escaped_list_separator<char> esc;
    size_t waka_no = 0;
    while (getline(fin, line)) {

      waka_play_t& play_info = waka_play_[waka_no];
      ++waka_no;

      int index = 0;
      tokenizer<escaped_list_separator<char> > tokens(line, esc);
      for (tokenizer<escaped_list_separator<char> >::iterator it =
             tokens.begin(); it != tokens.end(); ++it, ++index) {
        //fprintf(stderr, "%s, ", it->c_str());
        if (index == 0) {
          play_info.track_id = lexical_cast<int>(*it);
        } else if (index == 1) {
          play_info.start_frame = lexical_cast<int>(*it);
        } else if (index == 2) {
          play_info.play_interval = lexical_cast<int>(*it);
        }
      }
      if (index != 3) {
        return false;
      }
    }
    if (waka_no != 100) {
      return false;
    }

    return true;
  }

  void fileTracInfo(std::vector<int>& file_trac_info,
                    const char* fname) {

    std::ifstream fin(fname);
    std::string line;
    while (getline(fin, line)) {

      // !!! �Ƃ肠�����AID �͏��ԂɂȂ��ł���Ɖ��肵�Ă��܂���
      file_trac_info.push_back(atoi(line.c_str()));
    }
  }

  bool checkCd(void) {
    // cd_XXXX.txt �t�@�C���̒T��
    std::vector<std::string> cd_files;
    size_t n = findFiles(cd_files, dir_path_.c_str(),
                         sregex::compile("cd_[a-zA-Z0-9]+.txt"));
    if (n == 0) {
      return false;
    }

    // CD-ROM ���̓ǂݏo��
    std::vector<CdromPlayer::track_t> trac_info;
    bool exist_cd = cdrom_.getTrackList(trac_info);
    if (exist_cd == false) {
      return false;
    }

    // ���݂����t�@�C���ɑ΂��āACD-ROM �f�[�^�Ƃ̃`�F�b�N���s��
    for (std::vector<std::string>::iterator it = cd_files.begin();
         it != cd_files.end(); ++it) {

      // ��v���� waka_XXXX.txt �t�@�C���̓ǂݏo��
      std::vector<int> file_trac_info;
      fileTracInfo(file_trac_info, it->c_str());
      if (file_trac_info.size() != trac_info.size()) {
        // ���R�[�h������v���Ȃ�
        continue;
      }

      size_t index = 0;
      for (std::vector<CdromPlayer::track_t>::iterator fit = trac_info.begin();
           fit != trac_info.end(); ++fit, ++index) {

        // �t���[�������� �}1 �̍��������Ă��A�����Ƃ݂Ȃ�
        // Windows, Linux �ŁA�Ō�̃g���b�N�̒����� 1 �قȂ邽�߁B�ڍוs��
        if (abs(fit->frame_length - file_trac_info[index]) > 1) {
          break;
        }
      }
      if (index == trac_info.size()) {

        const char* path = it->c_str();
        const char* last_slash = strrchr(path, '/') + 1;
        std::string file_name = it->substr(last_slash - path,
                                           std::string::npos);
        std::string tag = file_name.substr(3);
        return readWakaInfo(("waka_" + tag).c_str());
      }
    }

    // ��v���� CD-ROM ��񂪌�����Ȃ�����
    return false;
  }

  void cd_play(size_t id) {
    // �w�肳�ꂽ�a�̃f�[�^�̍Đ�
    waka_play_t& waka = waka_play_[id];
    cdrom_.play(waka.track_id - 1, waka.start_frame, waka.play_interval);
  }

  void cd_stop(void) {
    cdrom_.stop();
  }

  bool checkData(std::string data_name) {

    common_ = CommonResources::getObject();

    // SaveDir/ + data_<data_name>.txt �����݂��邩�m�F
    std::string path = dir_path_ + "data_" + data_name + ".txt";

    // �t�@�C���̓��e��ǂݏo���Ċi�[
    std::ifstream fin(path.c_str());
    if (! fin.is_open()) {
      return false;
    }

    play_files_.clear();
    std::string line;
    while (getline(fin, line)) {
      play_files_.push_back(dir_path_ + line);
    }
    return true;
  }

  bool data_play(size_t id) {

    // ���\�[�X�̓ǂݏo��
    if (! common_->registerEffect(FloatingEffectId, play_files_[id].c_str())) {
      play_id_ = -1;
      return false;
    }

    // �Đ�
    // !!! �K�����Ȃ�...
    play_id_ = common_->playEffect(FloatingEffectId);
    return true;
  }

  void data_stop(void) {

    if (play_id_ >= 0) {
      common_->stopEffect(play_id_);
    }
  }
};


VoicePlayer::VoicePlayer(const char* directory)
  : pimpl(new pImpl(directory)) {
}


VoicePlayer::~VoicePlayer(void) {
}


std::vector<std::string> VoicePlayer::getResources(void) {

  // SaveDir/ + data_<data_name>.txt �� data_name ��Ԃ�
  std::vector<std::string> files;
  findFiles(files, pimpl->dir_path_.c_str(),
            sregex::compile("data_[a-zA-Z0-9_]+.txt"));

  std::vector<std::string> tag_names;
  tag_names.push_back("CD-ROM");

  for (std::vector<std::string>::iterator it = files.begin();
       it != files.end(); ++it) {
    //fprintf(stderr, "%s\n", it->c_str());

    // !!! ���������Ȃ̂ŁA������
    const char* path = it->c_str();
    const char* last_slash = strrchr(path, '/') + 1;
    std::string file_name = it->substr(last_slash - path,
                                       std::string::npos);
    std::string tag = file_name.substr(5, file_name.size() - 9);
    tag_names.push_back(tag);
  }
  return tag_names;
}


void VoicePlayer::setMute(void) {
  pimpl->mute_ = true;
}


bool VoicePlayer::checkResource(std::string resource) {
  if (pimpl->mute_) {
    return false;
  }

  // �g�p���\�[�X�̊m�F
  // !!! CD ���A�f�[�^���A�̐ݒ���m�F����
  pimpl->play_resource_ = resource;

  if (resource.empty()) {
    // CD-ROM ��p����
    return pimpl->checkCd();

  } else {
    // resource �������t�@�C����p����
    return pimpl->checkData(resource);
  }
}


void VoicePlayer::play(size_t id) {
  if (pimpl->mute_) {
    return;
  }

  if (pimpl->play_resource_.empty()) {
    pimpl->cd_play(id);
  } else {
    pimpl->data_play(id);
  }
}


void VoicePlayer::stop(void) {
  if (pimpl->mute_) {
    return;
  }

  // �Đ���~
  if (pimpl->play_resource_.empty()) {
    pimpl->cd_stop();
  } else {
    pimpl->data_stop();
  }
}
