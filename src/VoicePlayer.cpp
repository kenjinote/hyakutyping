/*!
  \file
  \brief 和歌の詠み上げ

  \author Satofumi KAMIMURA

  $Id$

  \todo CD-ROM の認識が完了したら、その旨をメッセージで出力すべき
  \todo エラー出力には、common_->debug か、LogManager を使う
  \todo 詠み上げ機能の On / Off に対応させる
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
  bool mute_;                   //!< 詠み上げを行わない

  int play_id_;                 //!< 効果音の再生チャネル
  std::vector<std::string> play_files_; //!< 再生データのファイル名

  /*!
    \brief 再生情報の格納用
  */
  typedef struct {
    int track_id;               //!< トラック番号
    size_t start_frame;         //!< 再生開始フレーム位置
    size_t play_interval;       //!< 再生フレーム間隔
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

    // 和歌情報の初期化
    for (size_t i = 0; i < 100; ++i) {
      waka_play_[i].track_id = InvalidTrack;
    }

    // 各和歌毎に、以下の内容を読み出す
    // - トラック
    // - 開始フレーム位置
    // - 再生フレーム間隔
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

      // !!! とりあえず、ID は順番にならんでいると仮定してしまおう
      file_trac_info.push_back(atoi(line.c_str()));
    }
  }

  bool checkCd(void) {
    // cd_XXXX.txt ファイルの探索
    std::vector<std::string> cd_files;
    size_t n = findFiles(cd_files, dir_path_.c_str(),
                         sregex::compile("cd_[a-zA-Z0-9]+.txt"));
    if (n == 0) {
      return false;
    }

    // CD-ROM 情報の読み出し
    std::vector<CdromPlayer::track_t> trac_info;
    bool exist_cd = cdrom_.getTrackList(trac_info);
    if (exist_cd == false) {
      return false;
    }

    // 存在したファイルに対して、CD-ROM データとのチェックを行う
    for (std::vector<std::string>::iterator it = cd_files.begin();
         it != cd_files.end(); ++it) {

      // 一致した waka_XXXX.txt ファイルの読み出し
      std::vector<int> file_trac_info;
      fileTracInfo(file_trac_info, it->c_str());
      if (file_trac_info.size() != trac_info.size()) {
        // レコード数が一致しない
        continue;
      }

      size_t index = 0;
      for (std::vector<CdromPlayer::track_t>::iterator fit = trac_info.begin();
           fit != trac_info.end(); ++fit, ++index) {

        // フレーム長さに ±1 の差があっても、同じとみなす
        // Windows, Linux で、最後のトラックの長さが 1 異なるため。詳細不明
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

    // 一致する CD-ROM 情報が見つからなかった
    return false;
  }

  void cd_play(size_t id) {
    // 指定された和歌データの再生
    waka_play_t& waka = waka_play_[id];
    cdrom_.play(waka.track_id - 1, waka.start_frame, waka.play_interval);
  }

  void cd_stop(void) {
    cdrom_.stop();
  }

  bool checkData(std::string data_name) {

    common_ = CommonResources::getObject();

    // SaveDir/ + data_<data_name>.txt が存在するか確認
    std::string path = dir_path_ + "data_" + data_name + ".txt";

    // ファイルの内容を読み出して格納
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

    // リソースの読み出し
    if (! common_->registerEffect(FloatingEffectId, play_files_[id].c_str())) {
      play_id_ = -1;
      return false;
    }

    // 再生
    // !!! 適当だなぁ...
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

  // SaveDir/ + data_<data_name>.txt の data_name を返す
  std::vector<std::string> files;
  findFiles(files, pimpl->dir_path_.c_str(),
            sregex::compile("data_[a-zA-Z0-9_]+.txt"));

  std::vector<std::string> tag_names;
  tag_names.push_back("CD-ROM");

  for (std::vector<std::string>::iterator it = files.begin();
       it != files.end(); ++it) {
    //fprintf(stderr, "%s\n", it->c_str());

    // !!! 同じ処理なので、くくる
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

  // 使用リソースの確認
  // !!! CD か、データか、の設定を確認する
  pimpl->play_resource_ = resource;

  if (resource.empty()) {
    // CD-ROM を用いる
    return pimpl->checkCd();

  } else {
    // resource が示すファイルを用いる
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

  // 再生停止
  if (pimpl->play_resource_.empty()) {
    pimpl->cd_stop();
  } else {
    pimpl->data_stop();
  }
}
