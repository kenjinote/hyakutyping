/*!
  \file
  \brief ���ʉ��̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$

  \todo �I�����ɂ́A������������� Mix_ChannelFinished() �𖳌������Ă���
*/

#include "SoundEffectManager.h"
#include "MixerInit.h"
#include "LockGuard.h"
#include "ExistFile.h"
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <map>

using namespace beego;


struct SoundEffectManager::pImpl : private MixerInit {

  class EffectInfo {
  public:
    std::string file_path;
    Mix_Chunk* effect;

    EffectInfo(void) : file_path(""), effect(NULL) {
    }
  };

  std::string error_message_;
  bool initialized;
  SDL_mutex* mutex;
  typedef std::map<int,EffectInfo> EffectMap;
  EffectMap effect_map;
  static size_t play_id;
  std::vector<size_t> channel_id;

  pImpl(void) :
    error_message_("no error"), initialized(false), mutex(SDL_CreateMutex()) {

    if (! isInitialized()) {
      return;
    }

    channel_id.resize(ChannelNum, InvalidId);
    Mix_ChannelFinished(channelDone);
    initialized = true;
  }

  ~pImpl(void) {
    if (! isInitialized()) {
      return;
    }
    Mix_HaltChannel(-1);
  }

  static pImpl* getObject(void) {
    static pImpl obj;
    return &obj;
  }

  static void freeMusResource(EffectMap& effect_maps, int effect_id,
                              bool erase = false) {
    EffectMap::iterator p = effect_maps.find(effect_id);
    if (p != effect_maps.end()) {
      if (p->second.effect) {
        Mix_FreeChunk(p->second.effect);
        p->second.effect = NULL;
      }
      if (erase) {
        effect_maps.erase(p);
      }
    }
  }

  static void channelDone(int channel) {
    static pImpl* obj = getObject();

    LockGuard guard(obj->mutex);
    obj->channel_id[channel] = InvalidId;
  }
};
size_t SoundEffectManager::pImpl::play_id = 1;


SoundEffectManager::SoundEffectManager(void) : pimpl(pImpl::getObject()) {
}


SoundEffectManager::~SoundEffectManager(void) {
}


const char* SoundEffectManager::what(void) {
  return pimpl->error_message_.c_str();
}


bool SoundEffectManager::isInitialized(void) {
  return pimpl->initialized;
}


bool SoundEffectManager::registerEffect(int effect_id, const char* file_path) {
  if (! isInitialized()) {
    return false;
  }

  // �t�@�C���̑��݃`�F�b�N
  if (! existFile(file_path)) {
    pimpl->error_message_ = std::string(file_path) + " no such file";
    return false;
  }

  // �����̂��̂Ɠ��� ID ���w�肳�ꂽ�ꍇ�A�������\�[�X���폜����
  LockGuard guard(pimpl->mutex);
  pImpl::freeMusResource(pimpl->effect_map, effect_id);

  // �����̂���ꏊ�̓o�^�ƃ��[�h
  pImpl::EffectInfo effect_info;
  effect_info.file_path = file_path;
  Mix_Chunk* effect = Mix_LoadWAV(file_path);
  if (! effect) {
    pimpl->error_message_ = Mix_GetError();
    return false;
  }
  effect_info.effect = effect;
  pimpl->effect_map[effect_id] = effect_info;

  return true;
}


void SoundEffectManager::unregisterEffect(int effect_id) {
  if (! isInitialized()) {
    // !!!
    return;
  }

  // �����̉��
  pImpl::freeMusResource(pimpl->effect_map, effect_id);
}


void SoundEffectManager::updateVolume(size_t percent, int channel_id) {
  if (! isInitialized()) {
    // !!!
    return;
  }

  if (percent > 100) {
    percent = 100;
  } else if (percent < 0) {
    percent = 0;
  }
  if (channel_id != AllChannel) {
    Mix_Volume(channel_id, MIX_MAX_VOLUME * percent / 100);
  } else {
    for (size_t i = 0; i < MixerInit::ChannelNum; ++i) {
      Mix_Volume(i, MIX_MAX_VOLUME * percent / 100);
    }
  }
}


int SoundEffectManager::play(int effect_id,
                        size_t fade_in_msec, int volume_percent) {
  if (! isInitialized()) {
    // !!!
    return InvalidId;
  }

  // !!! �Ƃ肠�����A�Đ�����
  pImpl::EffectMap::iterator it = pimpl->effect_map.find(effect_id);
  if (it == pimpl->effect_map.end()) {
    // !!!
    //pimpl->error_message_ = "no sound effect: %d\n", effect_id);
    return -1;
  }

  int id = Mix_PlayChannel(-1, it->second.effect, 0);
  if (id < 0) {
    // !!!
    return id;
  }

  LockGuard guard(pimpl->mutex);
  size_t playing_id = pImpl::play_id;
  pimpl->channel_id[id] = pImpl::play_id;
  ++pImpl::play_id;

  return playing_id;
}


void SoundEffectManager::stop(int serial_id, size_t fade_out_msec) {
  if (! isInitialized()) {
    // !!!
    return;
  }

  if (serial_id < 0) {
    Mix_FadeOutChannel(-1, fade_out_msec);
    return;
  }

  // �w�� ID �̌��ʉ����~
  int channel_id = -1;
  // !!! �ȉ��A�d�����Ă���̂ł�����ׂ�
  for (size_t i = 0; i < MixerInit::ChannelNum; ++i) {
    if (pimpl->channel_id[i] == static_cast<size_t>(serial_id)) {
      channel_id = i;
      break;
    }
  }
  Mix_FadeOutChannel(channel_id, fade_out_msec);
}


bool SoundEffectManager::nowPlaying(int serial_id) {
  if (! isInitialized()) {
    // !!!
    return false;
  }

  int channel_id = -1; // -1 �̂Ƃ��AMix_Playing() �͂ǂꂩ���Đ����Ȃ�ΐ^
  if (serial_id >= InvalidId) {

    LockGuard guard(pimpl->mutex);
    // !!! find �ɂ��ׂ����H
    for (size_t i = 0; i < MixerInit::ChannelNum; ++i) {
      if (pimpl->channel_id[i] == static_cast<size_t>(serial_id)) {
        channel_id = i;
        break;
      }
    }
  }
  return (Mix_Playing(channel_id) == 0) ? false : true;
}

#if 0
// !!! ~pImpl �ł̒�~�҂��Ŗ��Ȃ�����
// !!! GuiManager �ƈقȂ�A�����������Ŋ������Ă��邽��x1
void SoundEffectManager::terminate(void) {
}
#endif


void SoundEffectManager::setEachVolume(size_t effect_id, size_t percent) {

  // !!!
  // !!! ������
}
