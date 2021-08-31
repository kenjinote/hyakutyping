/*!
  \file
  \brief �f�[�^�̓ǂݏ���(���[�J��)

  \author Satofumi KAMIMURA

  $Id$

  \todo strftime ���g��
  \todo �L�^�f�[�^�̃o�[�W�����Ń}�C�N���ȏオ�قȂ�ꍇ�ɁA�g�p���Ȃ�
*/

#include "AccessRecordPC.h"
#include "ResourceDefinition.h"
#include "CommonResources.h"
#include "FindFiles.h"
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <fstream>

using namespace boost::xpressive;
using namespace boost;
using namespace beego;


struct AccessRecordPC::pImpl {
  const TypingMode mode_;
  std::string savedir_;

  pImpl(const TypingMode mode)
    : mode_(mode), savedir_(CommonResources::getObject()->savedir_) {
  }
};


AccessRecordPC::AccessRecordPC(const TypingMode mode)
  : pimpl(new pImpl(mode)) {
}


AccessRecordPC::~AccessRecordPC(void) {
}


bool AccessRecordPC::save(const TypingRecorder::GameSettings& data) {

  // �t�@�C�����̐���
  time_t timer;
  time(&timer);
  struct tm* today = localtime(&timer);
  char file_name[80];
  const char mode_ch = (pimpl->mode_ == ShimonokuTyping) ? 's' : 'k';

  sprintf(file_name, "%c_%02d%02d%02d_%s.txt", mode_ch,
          (today->tm_year % 100), (today->tm_mon + 1), today->tm_mday,
          data.user_name.c_str());

  std::string output_file = pimpl->savedir_ + file_name;

  // �f�[�^�̋L�^
  std::string record_data = TypingRecorder::encodeGameData(data);
  std::ofstream fout(output_file.c_str(), std::ios::app);
  if (! fout.is_open()) {
    // !!! �G���[���O�̏o��
    return false;
  }

  fout << record_data << std::endl;

  return true;
}


bool AccessRecordPC::load(std::vector<TypingRecorder::GameSettings>& data,
                          const char* user_name,
                          time_t from, time_t to, size_t max_num) {

  // ���[�h�ƃ��[�U������v����t�@�C���̒T��
  std::vector<std::string> match_files;
  findFiles(match_files, pimpl->savedir_.c_str(),
            sregex::compile("[sk]_[0-9]+_[a-zA-Z0-9]+.txt"));

  // �t�@�C�����ɁA�͈͓��̉\�������邩�𔻕ʂ��A�ǂݏo�����s��
  std::vector<std::string> data_lines;
  for (std::vector<std::string>::iterator it = match_files.begin();
       it != match_files.end(); ++it) {

    // �t�@�C�����̃p�[�X
    const char* path = it->c_str();
    const char* last_slash = strrchr(path, '/') + 1;
    std::string file_name = it->substr(last_slash - path, std::string::npos);

    // !!! ���[�U���̎擾�ȊO�́A�s�v����
    size_t year = 2000 + lexical_cast<int>(file_name.substr(2, 2));
    size_t month = lexical_cast<int>(file_name.substr(4, 2));
    size_t day = lexical_cast<int>(file_name.substr(6, 2));
    size_t length = file_name.size();
    std::string player_name = file_name.substr(9, length - 13);

    if (file_name.empty()) {
      // �t�@�C��������@
      continue;
    }
    if (file_name[0] != ((pimpl->mode_ == ShimonokuTyping) ? 's' : 'k')) {
      // ���[�h���Ⴄ
      continue;
    }

    if (player_name.compare(user_name)) {
      // ���[�U�����Ⴄ
      continue;
    }

    // �t�@�C���͈͎̔w��
    struct tm file_tm;
    file_tm.tm_sec = 0;
    file_tm.tm_min = 0;
    file_tm.tm_hour = 0;
    file_tm.tm_mday = static_cast<int>(day);
    file_tm.tm_mon = static_cast<int>(month - 1);
    file_tm.tm_year = static_cast<int>(year - 1900);
    file_tm.tm_isdst = 0;
    time_t file_ctime_first = mktime(&file_tm);
    time_t file_ctime_last = file_ctime_first + (60 * 60 * 24);

    if ((to < file_ctime_first) || (from > file_ctime_last)) {
      // �w��͈͊O
      continue;
    }

    // �s���Ƀf�[�^��ǂݏo��
    std::ifstream fin(path);
    std::string line;
    while (getline(fin, line)) {
      if (line.empty()) {
        continue;
      }
      data_lines.push_back(line);
    }
  }

  // �\�[�g
  sort(data_lines.begin(), data_lines.end());

  // max_num �����̓K�p
  if (data_lines.size() > max_num) {
    data_lines.resize(max_num);
  }
  // !!! ���]�A�����
  //reverse(data_lines.begin(), data_lines.end());

  TypingRecorder::GameSettings setting;
  for (std::vector<std::string>::iterator lit = data_lines.begin();
       lit != data_lines.end(); ++lit) {

    // GameSettings �ւ̕ϊ�
    escaped_list_separator<char> esc;
    tokenizer<escaped_list_separator<char> > tokens(*lit, esc);

    setting.waka_num = 1;

    // !!! �T�C�Y�̊m�F�������āA++ ���Ȃ���f�[�^�����o���ׂ�����
    // !!! �܂��A�T�C�Y�̊m�F�͌��

    tokenizer<escaped_list_separator<char> >::iterator it = tokens.begin();

    setting.time = lexical_cast<time_t>(*it); ++it;
    setting.major_version = *it; ++it;
    setting.minor_version = *it; ++it;
    setting.micro_version = *it; ++it;

    // Major, Minor �o�[�W�������قȂ�f�[�^�͈���Ȃ�
    if (0) {
      continue;
    }

    setting.mode = pimpl->mode_; ++it;
    setting.user_name = user_name; ++it;
    setting.rand_seed = lexical_cast<long>(*it); ++it;
    setting.score.miss_types = lexical_cast<int>(*it); ++it;
    setting.score.delay_msec = lexical_cast<int>(*it); ++it;
    setting.score.elapse_msec = lexical_cast<int>(*it); ++it;
    setting.score.types = lexical_cast<int>(*it); ++it;
    setting.waka_num = lexical_cast<int>(*it); ++it;
    setting.kaminoku_speed = lexical_cast<int>(*it); ++it;
    setting.shimonoku_speed = lexical_cast<int>(*it); ++it;

    data.push_back(setting);
  }

  return true;
}
