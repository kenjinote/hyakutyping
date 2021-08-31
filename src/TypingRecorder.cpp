/*!
  \file
  \brief タイピング情報の記録

  \author Satofumi KAMIMURA

  $Id$

  \todo total_types の記録値がおかしいのを確認して修正する
*/

#include "TypingRecorder.h"
#include "hyakutyping.h"
#include "SystemDefinition.h"
#include <boost/lexical_cast.hpp>
#include <ctype.h>
#include <time.h>


using namespace boost;


struct TypingRecorder::pImpl {
  GameSettings data_;
  size_t waka_num_;
  double start_delay_;
  double typing_speed_;
  size_t miss_types_;

  pImpl(void)
    : waka_num_(0), start_delay_(0.0), typing_speed_(0.0), miss_types_(0) {
  }

  void initData(void) {
    data_.rand_seed = 1;
    data_.mode = ShimonokuTyping;
    data_.waka.clear();
    data_.waka_num = 0;
    data_.kaminoku_speed = KaminokuPlaying;
    data_.shimonoku_speed = ShimonokuPlaying;

  }
};


TypingRecorder::TypingRecorder(void) : pimpl(new pImpl) {
}


TypingRecorder::~TypingRecorder(void) {
}


void TypingRecorder::recordGame(int rand_seed, TypingMode mode) {
  pimpl->initData();

  pimpl->data_.rand_seed = rand_seed;
  pimpl->data_.mode = mode;
}


void TypingRecorder::recordWaka(size_t no) {
  WakaTyping waka;
  waka.no = no;
  pimpl->data_.waka.push_back(waka);
}


void TypingRecorder::evaluate(const char* user_name) {

  // 記録時間、ユーザ名
  pimpl->data_.time = time(NULL);
  pimpl->data_.user_name = user_name;

  // 和歌の数
  pimpl->waka_num_ = pimpl->data_.waka.size();

  // タイプ速度、タイピング開始遅延、訂正タイプ数、の計算
  enum { SystemTypes = 5 };
  size_t types = 0;
  size_t spent = 0;
  size_t miss = 0;
  size_t delay = 0;
  for (std::vector<WakaTyping>::iterator it = pimpl->data_.waka.begin();
       it != pimpl->data_.waka.end(); ++it) {
    size_t keys_size = it->keys.size();
    types += keys_size - SystemTypes - 2;
    spent += it->keys[keys_size - 1 - 3].ticks - it->keys[1].ticks;

    size_t valid_input_ticks = 0;
    size_t complete_times = 0;
    int exist_chars = 0;
    for (std::vector<KeyTiming>::iterator key_it = it->keys.begin();
         key_it != it->keys.end(); ++key_it) {
      if (key_it->ch == BackSpace) {
        ++miss;
        --exist_chars;

      } else if (key_it->ch == Clear) {
        ++miss;
        exist_chars = 0;

      } else if (key_it->ch == Return) {
        exist_chars = 0;

      } else if (key_it->ch == Complete) {
        ++complete_times;

      } else {
        if ((exist_chars <= 0) && (complete_times == 0)) {
          valid_input_ticks = key_it->ticks;
        }
        ++exist_chars;
      }
    }
    miss -= SystemTypes + ((pimpl->data_.mode == KimarijiTyping) ? -1 : 0);
    delay += valid_input_ticks;
  }

  // タイピング速度
  pimpl->data_.score.types = types;
  pimpl->typing_speed_ = 1.0 * types / (spent / 1000.0);

  // 訂正タイプ数
  pimpl->miss_types_ = miss;
  pimpl->data_.score.miss_types = pimpl->miss_types_;

  // 入力開始遅延
  pimpl->start_delay_ = 1.0 * delay / 1000.0;
  pimpl->data_.score.delay_msec = static_cast<int>(1000 * pimpl->start_delay_);

  // 経過時間
  pimpl->data_.score.elapse_msec = spent;

  // 和歌情報
  pimpl->data_.waka_num = pimpl->data_.waka.size();

  // !!!
}


size_t TypingRecorder::getWakaNum(void) {
  return pimpl->waka_num_;
}


double TypingRecorder::getStartDelay(void) {
  return pimpl->start_delay_ / pimpl->waka_num_;
}


double TypingRecorder::getTypingSpeed(void) {
  return pimpl->typing_speed_;
}


size_t TypingRecorder::getMissTypes(void) {
  return pimpl->miss_types_;
}


void TypingRecorder::recordTyping(char ch, size_t ticks) {

  if (pimpl->data_.waka.empty()) {
    return;
  }

  WakaTyping& last = pimpl->data_.waka.back();
  KeyTiming key_timing;
  key_timing.ch = ch;
  key_timing.ticks = ticks;
  last.keys.push_back(key_timing);
}


TypingRecorder::GameSettings& TypingRecorder::getRecordData(void) const {
  return pimpl->data_;
}


std::string TypingRecorder::encodeGameData(const GameSettings& data) {

  // ゲーム設定のエンコード

  // time_t
  std::string output = lexical_cast<std::string>(data.time);

  // version (major, minor, micro)
  output += std::string(",")
    + MAJOR_VERSION + "," + MINOR_VERSION + "," + MICRO_VERSION;

  // TypingMode
  output += (data.mode == ShimonokuTyping) ? ",s" : ",k";

  // user_name
  output += "," + data.user_name;

  // rand_seed
  output += "," + lexical_cast<std::string>(data.rand_seed);

  // ミスタイプ数, 入力遅延, 経過時間
  output += "," + lexical_cast<std::string>(data.score.miss_types);
  output += "," + lexical_cast<std::string>(data.score.delay_msec);
  output += "," + lexical_cast<std::string>(data.score.elapse_msec);

  // 総タイプ数
  output += "," + lexical_cast<std::string>(data.score.types);

  // waka の数
  output += "," + lexical_cast<std::string>(data.waka.size());

  // ゲーム速度
  output += "," + lexical_cast<std::string>(data.kaminoku_speed);
  output += "," + lexical_cast<std::string>(data.shimonoku_speed);

  // 和歌毎のタイピングデータ
  for (std::vector<WakaTyping>::const_iterator it = data.waka.begin();
       it != data.waka.end(); ++it) {

    // キー入力数
    output += "," + lexical_cast<std::string>(it->keys.size());

    for (std::vector<KeyTiming>::const_iterator key_it = it->keys.begin();
         key_it != it->keys.end(); ++key_it) {

      // 入力キー, 入力時刻
      output +=
        "," + lexical_cast<std::string>(static_cast<int>(key_it->ch)) +
        "," + lexical_cast<std::string>(key_it->ticks);
    }
  }

  return output;
}


void TypingRecorder::print(void) {

  fprintf(stderr, "rand_seed = %d\n", pimpl->data_.rand_seed);
  fprintf(stderr, "mode = %d\n", pimpl->data_.mode);

  for (std::vector<WakaTyping>::iterator it = pimpl->data_.waka.begin();
       it != pimpl->data_.waka.end(); ++it) {
    fprintf(stderr, "no = %d\n", it->no);

    for (std::vector<KeyTiming>::iterator key_it = it->keys.begin();
         key_it != it->keys.end(); ++key_it) {
      char ch = key_it->ch;
      if (isprint(ch)) {
        fprintf(stderr, "%c:%d, ", ch, key_it->ticks);
      } else {
        switch (ch) {
        case Clear:
          fprintf(stderr, "[clear]:%d, ", key_it->ticks);
          break;

        case BackSpace:
          fprintf(stderr, "[bs]:%d, ", key_it->ticks);
          break;

        case Return:
          fprintf(stderr, "[return]:%d, ", key_it->ticks);
          break;

        case Complete:
          fprintf(stderr, "[complete]:%d, ", key_it->ticks);
          break;
        }
      }
    }
    fprintf(stderr, "\n");
  }
}
