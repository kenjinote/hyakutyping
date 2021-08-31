/*!
  \file
  \brief �a�̕��т̃V���b�t���N���X

  \author Satofumi KAMIMURA

  $Id$

  \todo ���܂莚�̂Ƃ��ɂ́A���܂莚���O���[�v��������Ń\�[�g����
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

    // !!! 6 �ł��߂Ȃ̂łƂ肠���� +2 ���Ă����B������ strncmp ���C��
    KimarijiMax = 6 + 2,        // �a�̂̌��܂莚�́A�ő�łU����
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

    // !!! n ���ő�f�[�^���ȏ�ɂȂ�Ȃ��悤�ɂ��邱��
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

        // ���̔�r�� 0x0 ��p���Ĕ�r���s��Ȃ����߂̏���
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

    // �x���ɑΏ�
    //if (YomibitoName || KanjiWaka);
  }

  void shimonokuTypingSort(std::vector<int>& waka) {

    // �a�̗̂��������
    waka.clear();
    for (int i = 0; i < 100; ++i) {
      waka.push_back(i);
    }

    // �����̏�����
    uniform_int<> range(0, 99);
    variate_generator<mt19937,uniform_int<> >
      rand(mt19937(static_cast<boost::uint32_t>(seed)), range);

    // �V���b�t��
    if (seed != NoShuffleSeed) {
      for (int i = 0; i < 99; ++i) {
        int swap_index = rand();
        std::swap(waka[i], waka[swap_index]);
      }

      // �d�_���K�̘a�̂��w�肳�ꂽ�ꏊ����O�Ɉړ�������
      if (waka_no != pImpl::InvalidNo) {
        std::vector<int>::iterator p = find(waka.begin(), waka.end(), waka_no);
        if (p == waka.end()) {
          // ���肦�Ȃ����ꉞ
          return;
        }
        int swap_index = rand() % range_to;
        std::swap(*p, waka[swap_index]);
      }
    }
  }

  void kimarijiTypingSort(std::vector<int>& waka) {

    // ���܂莚�̃O���[�v�����
    createKimarijiGroup();

    if (seed != NoShuffleSeed) {
      // ���܂莚�̎w��O���[�v���܂܂��悤�ɂ��鏈��
      // !!!

      // ���܂莚�O���[�v�̃V���b�t��
      // !!!

      // �T�u�O���[�v�̃V���b�t��
      // !!!
    }

    // ���܂莚�̃O���[�v�����
    waka.clear();
    // !!! waka �ɑ΂��ă}�[�W�������ʂ�������
    // !!! �����������\�b�h�� STL �ɂ������C������
    // !!! �Ƃ肠�����A�ׂ��ɍ��
    // !!! �������ɁAtypedef ���ׂ����H
    for (WakaGroup::iterator git = kimariji_group.begin();
         git != kimariji_group.end(); ++git) {
      for (std::vector<int>::iterator sit = git->begin();
           sit != git->end(); ++sit) {
        waka.push_back(*sit);
        //fprintf(stderr, "%d, ", *sit);
      }
    }
  }

  // ���܂莚�̃O���[�v�����
  void createKimarijiGroup(void) {

    // �����ς݂�������߂�
    if (! kimariji_group.empty()) {
      return;
    }

    // ���܂莚���Ƀ\�[�g����
    std::set<KimarijiOrder> kimariji_order;
    for (int i = 0; i < 100; ++i) {
      kimariji_order.insert(KimarijiOrder(i));
    }

    // ���܂莚�������喈�ɃT�u�O���[�v�������s��
    std::vector<int> sub_group;
    size_t kimariji_length = 1;
    for (std::set<KimarijiOrder>::iterator it = kimariji_order.begin();
         it != kimariji_order.end(); ++it) {

      // �O���[�v����Ȃ�a�̂�o�^����
      int now_no = it->getNo();
      if (sub_group.empty()) {
        sub_group.push_back(now_no);
        kimariji_length = 1;
        continue;
      }

      // ���̘a�̂Ɠ�����������т�����΁A�������܂莚�O���[�v�ɓo�^����
      int prev_no = sub_group.front();

      bool matched = false;
      for (; kimariji_length < KimarijiMax; ++kimariji_length) {
        // !!! ����������ƁA�������肷��C������
        bool ret = (KimarijiOrder::strncmp(prev_no, now_no, kimariji_length)
                    == 0) ? true : false;
        matched |= ret;
        if (ret == false) {
          break;
        }
      }
      --kimariji_length;

      if (matched) {
        // ���܂莚�������Ƃ݂Ȃ��ăT�u�O���[�v�ɓo�^
        sub_group.push_back(now_no);

      } else {
        // �T�u�O���[�v��o�^���ċ�ɂ���
        kimariji_group.push_back(sub_group);

#if 0
        // !!! �Ƃ肠�����A���т�\�������Ċm�F����
        for (std::vector<int>::iterator test_it = sub_group.begin();
             test_it != sub_group.end(); ++test_it) {
          fprintf(stderr, "%d, ", *test_it);
        }
        fprintf(stderr, "\n");
#endif
        sub_group.clear();

        // !!! ��̕��Ƌ��ʎ����A�����ꂭ����
        sub_group.push_back(now_no);
        kimariji_length = 1;
      }
    }
    kimariji_group.push_back(sub_group);
    // !!! ������...�B�K���Ɋ֐������܂��傤
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
    // !!! kimarijiTypingSort() ����������܂ł̎b�菈��
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

  // ���肦�Ȃ��͂������A�ꉞ
  std::vector<int> not_found;
  return not_found;
}
