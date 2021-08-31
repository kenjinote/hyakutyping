/*!
  \file
  \brief �w�i�Ȃ̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include "MusicManager.h"
#include "MixerInit.h"
#include "ThreadCreator.h"
#include "LockGuard.h"
#include "ExistFile.h"
#include "Delay.h"
#include <SDL_mixer.h>
#include <string>
#include <deque>
#include <map>

using namespace beego;


struct MusicManager::pImpl : private MixerInit {

  bool initialized;
  size_t fade_in_msec;
  size_t fade_out_msec;
  static bool now_playing;
  int current_music_id;

  class MusicInfo {
  public:
    std::string file_path;
    Mix_Music* music;

    MusicInfo(void) : file_path(""), music(NULL) {
    }
  };
  typedef std::map<int,MusicInfo> MusicMap;
  MusicMap music_map;

  SDL_mutex* mutex;

  class ChangeOrder {
  public:
    int next_music_id;
    int play_times;

    ChangeOrder(int next_id, int times)
      : next_music_id(next_id), play_times(times) {
    }
  };
  typedef std::deque<ChangeOrder> ChangeOrderList;
  ChangeOrderList change_order;

  class ThreadArgs {
  public:
    pImpl* own_obj;
    ThreadArgs(pImpl* obj) : own_obj(obj) {
    }
  };
  ThreadArgs thread_args;
  ThreadCreator thread;

  pImpl(void) : initialized(false), fade_in_msec(0), fade_out_msec(0),
                current_music_id(0), mutex(SDL_CreateMutex()),
                thread_args(this), thread(change_thread, &thread_args) {

    if (! isInitialized()) {
      return;
    }
    initialized = true;
  }

  ~pImpl(void) {

    // �Đ��X���b�h�̊����҂�
    thread.wait();

    // ���y�����̉��
    // !!! freeMusResource() ���g���悤�ɕύX���ׂ�����
    for (MusicMap::iterator it = music_map.begin();
         it != music_map.end(); ++it) {
      if (it->second.music) {
        Mix_FreeMusic(it->second.music);
      }
    }

    // �X���b�h�̏I���҂�
    thread.wait();

    SDL_DestroyMutex(mutex);
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }

  static int change_thread(void* args) {
    ThreadArgs* info = static_cast<ThreadArgs*>(args);
    SDL_mutex* mutex = info->own_obj->mutex;
    ChangeOrderList& change_order = info->own_obj->change_order;
    MusicMap& music_map = info->own_obj->music_map;
    size_t& fade_in_msec = info->own_obj->fade_in_msec;
    size_t& fade_out_msec = info->own_obj->fade_out_msec;
    bool& now_playing = info->own_obj->now_playing;
    int& current_music_id = info->own_obj->current_music_id;

    while (true) {
      LockMutex(mutex);
      if (change_order.empty()) {
        UnlockMutex(mutex);
        return 0;
      }

      // �w�߂̎󂯎��B�Ō�̎w�߂݈̂���
      ChangeOrder order = change_order.back();
      int play_times = order.play_times;
      int next_music_id = order.next_music_id;
      change_order.clear();

      pImpl::MusicMap::iterator p;
      if (play_times != 0) {
        p = music_map.find(next_music_id);
        if (p == music_map.end()) {
          // �Ȃ�������Ȃ�
          // !!! �G���[�o��
          now_playing = false;
          UnlockMutex(mutex);
          return -1;
        }
      }

      // �t�F�[�h�C������������̂�҂�
      // Mix_FadeInMusic() �̓��쒆�� Mix_FadeOutMusic() �����삵�Ȃ�����
      // !!! �����ɂ́Afade_out_msec == 0 �Ȃ�Α����ɒ�~������ׂ������A�ۗ�
      UnlockMutex(mutex);

      while (Mix_FadingMusic() != MIX_NO_FADING) {
        delay(10);
      }
      LockMutex(mutex);
      Mix_FadeOutMusic(fade_out_msec);
      UnlockMutex(mutex);

      // �t�F�[�h�A�E�g�̊�����҂��āA���̃t�F�[�h���n�߂�
      while (Mix_PlayingMusic()) {
        delay(10);
      }

      LockMutex(mutex);
      if (current_music_id != next_music_id) {
        pImpl::freeMusResource(music_map, current_music_id);
      }
      if (play_times == 0) {
        // �Ȃ̒�~���s���A�߂�
        now_playing = false;
        UnlockMutex(mutex);
        continue;
      }

      // ���y�t�@�C���̓ǂݏo��
      p = music_map.find(next_music_id);
      if (p == music_map.end()) {
        // !!! �t�F�[�h���ɋȂ�������ꂽ�ꍇ�H
        continue;
      }
      pImpl::MusicInfo& music_info = p->second;
      if (music_info.music == NULL) {
        music_info.music = Mix_LoadMUS(music_info.file_path.c_str());
        if (music_info.music == NULL) {
          // !!! �G���[�o��
          now_playing = false;
          UnlockMutex(mutex);
          continue;
        }
      }

      // ���̋Ȃ��Đ�
      now_playing = true;
      Mix_HookMusicFinished(pImpl::playFinished);
      Mix_FadeInMusic(music_info.music, play_times, fade_in_msec);
      current_music_id = next_music_id;
      UnlockMutex(mutex);
    }
  }

  static void playFinished(void) {
    now_playing = false;
  }

  static void freeMusResource(MusicMap& music_maps, int music_id,
                              bool erase = false) {
    MusicMap::iterator p = music_maps.find(music_id);
    if (p != music_maps.end()) {
      if (p->second.music) {
        Mix_FreeMusic(p->second.music);
        p->second.music = NULL;
      }
      if (erase) {
        music_maps.erase(p);
      }
    }
  }
};
bool MusicManager::pImpl::now_playing = false;


MusicManager::MusicManager(void) : pimpl(pImpl::getObject()) {
}


MusicManager::~MusicManager(void) {
}


bool MusicManager::isInitialized(void) {
  if (pimpl) {
    return pimpl->initialized;
  } else {
    return false;
  }
}


bool MusicManager::registerMusic(int music_id, const char* file_path) {
  if (! isInitialized()) {
    return false;
  }

  // �t�@�C���̑��݃`�F�b�N
  if (! existFile(file_path)) {
    return false;
  }

  pImpl::MusicInfo music_info;
  music_info.file_path = file_path;

  // �����̂��̂Ɠ��� ID ���w�肳�ꂽ�ꍇ�A�������\�[�X���폜����
  LockGuard guard(pimpl->mutex);
  pImpl::freeMusResource(pimpl->music_map, music_id);

  // �Ȏ����̂���ꏊ�݂̂�o�^���A���[�h�͎��ۂ̍Đ����ɍs��
  pimpl->music_map[music_id] = music_info;

  return true;
}


void MusicManager::unregisterMusic(int music_id) {
  if (! isInitialized()) {
    return;
  }

  LockGuard guard(pimpl->mutex);
  pImpl::freeMusResource(pimpl->music_map, music_id, true);
}


void MusicManager::updateVolume(size_t percent) {
  if (! isInitialized()) {
    return;
  }

  if (percent > 100) {
    percent = 100;
  } else if (percent < 0) {
    percent = 0;
  }
  Mix_VolumeMusic(MIX_MAX_VOLUME * percent / 100);
}


void MusicManager::setNextMusic(int next_music_id, int play_times) {
  if (! isInitialized()) {
    return;
  }

  LockGuard guard(pimpl->mutex);

  // ��n�߂��ƌ��Ȃ�
  pImpl::now_playing = true;

  pimpl->change_order.push_back(pImpl::ChangeOrder(next_music_id, play_times));
  pimpl->thread.run(1);
}


void MusicManager::stop(size_t fade_out_msec) {
  if (! isInitialized()) {
    return;
  }

  setFadeOutEffect(fade_out_msec);
  setNextMusic(0, 0);
}


void MusicManager::halt(void) {
  if (! isInitialized()) {
    return;
  }

  Mix_HaltMusic();
  LockGuard guard(pimpl->mutex);
  pImpl::now_playing = false;
}


void MusicManager::setFadeInEffect(size_t effect_msec) {
  if (! isInitialized()) {
    // !!!
    return;
  }

  LockGuard guard(pimpl->mutex);
  pimpl->fade_in_msec = effect_msec;
}


void MusicManager::setFadeOutEffect(size_t effect_msec) {
  if (! isInitialized()) {
    return;
  }

  LockGuard guard(pimpl->mutex);
  pimpl->fade_out_msec = effect_msec;
}


int MusicManager::getCurrentMusicId(void) {
  if (! isInitialized()) {
    return -1;
  }

  LockGuard guard(pimpl->mutex);
  return pimpl->current_music_id;
}


bool MusicManager::nowPlaying(void) {
  if (! isInitialized()) {
    fprintf(stderr, "not initialized.\n");
    return false;
  }

  LockGuard guard(pimpl->mutex);
  return ((Mix_PlayingMusic() != 0) || pImpl::now_playing) ? true : false;
}
