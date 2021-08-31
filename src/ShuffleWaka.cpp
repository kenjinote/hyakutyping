/*!
  \file
  \brief 和歌並びのシャッフルクラス

  \author Satofumi KAMIMURA

  $Id$

  \todo 決まり字のときには、決まり字をグループ化した上でソートする
*/

#include "ShuffleWaka.h"
#include "WakaData.h"
#include <algorithm>
#include <set>
#include <boost/random.hpp>

using namespace boost;


struct ShuffleWaka::pImpl {
  enum {
    InvalidNo = -1,

    // !!! 6 でだめなのでとりあえず +2 しておく。いずれ strncmp を修正
    KimarijiMax = 6 + 2,        // 和歌の決まり字は、最大で６文字
  };
  TypingMode typing_mode;
  int seed;
  int waka_no;
  int range_to;
  typedef std::vector<std::vector<int> > WakaGroup;
  WakaGroup kimariji_group;

  class KimarijiOrder {
    int waka_no;

  public:
    KimarijiOrder(void) : waka_no(0) {
    }

    KimarijiOrder(int no) : waka_no(no) {
    }

    bool operator<(const KimarijiOrder& rhs) const {
      return (strncmp(waka_no, rhs.waka_no, KimarijiMax) < 0) ? true : false;
    }

    int getNo(void) const {
      return waka_no;
    }

    // !!! n が最大データ長以上にならないようにすること
    static int strncmp(int no1, int no2, size_t n) {
      if (no1 == no2) {
        return 0;
      }

      enum { Left = 0, Right = 1 };
      const Uint16* ch[2];
      ch[Left] = KanaWaka[no1][0];
      ch[Right] = KanaWaka[no2][0];

      for (size_t i = 0; i < n; ++i) {

        int cmp = *ch[Left] - *ch[Right];
        if (cmp != 0) {
          return cmp;
        }

        // 次の比較で 0x0 を用いて比較を行わないための準備
        for (int i = 0; i < 2; ++i) {
          ++ch[i];
          if (*ch[i] == 0x0) {
            ++ch[i];
          }
        }
      }
      return 0;
    }
  };

  pImpl(TypingMode mode, int random_seed)
    : typing_mode(mode), seed(random_seed), waka_no(InvalidNo), range_to(0) {

    // 警告に対処
    //if (YomibitoName || KanjiWaka);
  }

  void shimonokuTypingSort(std::vector<int>& waka) {

    // 和歌の列を初期化
    waka.clear();
    for (int i = 0; i < 100; ++i) {
      waka.push_back(i);
    }

    // 乱数の初期化
    uniform_int<> range(0, 99);
    variate_generator<mt19937,uniform_int<> >
      rand(mt19937(static_cast<boost::uint32_t>(seed)), range);

    // シャッフル
    if (seed != NoShuffleSeed) {
      for (int i = 0; i < 99; ++i) {
        int swap_index = rand();
        std::swap(waka[i], waka[swap_index]);
      }

      // 重点練習の和歌を指定された場所より手前に移動させる
      if (waka_no != pImpl::InvalidNo) {
        std::vector<int>::iterator p = find(waka.begin(), waka.end(), waka_no);
        if (p == waka.end()) {
          // ありえないが一応
          return;
        }
        int swap_index = rand() % range_to;
        std::swap(*p, waka[swap_index]);
      }
    }
  }

  void kimarijiTypingSort(std::vector<int>& waka) {

    // 決まり字のグループを作る
    createKimarijiGroup();

    if (seed != NoShuffleSeed) {
      // 決まり字の指定グループが含まれるようにする処理
      // !!!

      // 決まり字グループのシャッフル
      // !!!

      // サブグループのシャッフル
      // !!!
    }

    // 決まり字のグループを作る
    waka.clear();
    // !!! waka に対してマージした結果を代入する
    // !!! そういうメソッドが STL にあった気がする
    // !!! とりあえず、べたに作る
    // !!! さすがに、typedef すべきか？
    for (WakaGroup::iterator git = kimariji_group.begin();
         git != kimariji_group.end(); ++git) {
      for (std::vector<int>::iterator sit = git->begin();
           sit != git->end(); ++sit) {
        waka.push_back(*sit);
        //fprintf(stderr, "%d, ", *sit);
      }
    }
  }

  // 決まり字のグループを作る
  void createKimarijiGroup(void) {

    // 生成済みだったら戻る
    if (! kimariji_group.empty()) {
      return;
    }

    // 決まり字毎にソートする
    std::set<KimarijiOrder> kimariji_order;
    for (int i = 0; i < 100; ++i) {
      kimariji_order.insert(KimarijiOrder(i));
    }

    // 決まり字が同じ句毎にサブグループ分けを行う
    std::vector<int> sub_group;
    size_t kimariji_length = 1;
    for (std::set<KimarijiOrder>::iterator it = kimariji_order.begin();
         it != kimariji_order.end(); ++it) {

      // グループが空なら和歌を登録する
      int now_no = it->getNo();
      if (sub_group.empty()) {
        sub_group.push_back(now_no);
        kimariji_length = 1;
        continue;
      }

      // 次の和歌と同じ文字列並びがあれば、同じ決まり字グループに登録する
      int prev_no = sub_group.front();

      bool matched = false;
      for (; kimariji_length < KimarijiMax; ++kimariji_length) {
        // !!! もうちょっと、すっきりする気がする
        bool ret = (KimarijiOrder::strncmp(prev_no, now_no, kimariji_length)
                    == 0) ? true : false;
        matched |= ret;
        if (ret == false) {
          break;
        }
      }
      --kimariji_length;

      if (matched) {
        // 決まり字が同じとみなしてサブグループに登録
        sub_group.push_back(now_no);

      } else {
        // サブグループを登録して空にする
        kimariji_group.push_back(sub_group);

#if 0
        // !!! とりあえず、並びを表示させて確認する
        for (std::vector<int>::iterator test_it = sub_group.begin();
             test_it != sub_group.end(); ++test_it) {
          fprintf(stderr, "%d, ", *test_it);
        }
        fprintf(stderr, "\n");
#endif
        sub_group.clear();

        // !!! 上の方と共通実装、いずれくくる
        sub_group.push_back(now_no);
        kimariji_length = 1;
      }
    }
    kimariji_group.push_back(sub_group);
    // !!! 長いな...。適当に関数化しましょう
  }
};


ShuffleWaka::ShuffleWaka(TypingMode mode, int seed)
  : pimpl(new pImpl(mode, seed)) {
}


ShuffleWaka::~ShuffleWaka(void) {
}


void ShuffleWaka::setSomeWaka(int no, int range_to) {
  pimpl->waka_no = no;
  pimpl->range_to = range_to;
}


void ShuffleWaka::shuffle(std::vector<int>& waka) {

  switch (pimpl->typing_mode) {
  case ShimonokuTyping:
    pimpl->shimonokuTypingSort(waka);
    break;

  case KimarijiTyping:
    // !!! kimarijiTypingSort() を実装するまでの暫定処理
    pimpl->shimonokuTypingSort(waka);
    //pimpl->kimarijiTypingSort(waka);
    break;
  }
}


std::vector<int> ShuffleWaka::getKimarijiGroup(int no) {
  pimpl->createKimarijiGroup();

  for (pImpl::WakaGroup::iterator it = pimpl->kimariji_group.begin();
       it != pimpl->kimariji_group.end(); ++it) {
    std::vector<int>::iterator p = find(it->begin(), it->end(), no);
    if (p != it->end()) {
      return *it;
    }
  }

  // ありえないはずだが、一応
  std::vector<int> not_found;
  return not_found;
}
