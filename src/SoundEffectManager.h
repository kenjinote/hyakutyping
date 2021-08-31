#ifndef SOUND_EFFECT_MANAGER_H
#define SOUND_EFFECT_MANAGER_H

/*!
  \file
  \brief 効果音の管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief 効果音の管理クラス
  */
  class SoundEffectManager {
    SoundEffectManager(const SoundEffectManager& rhs);
    SoundEffectManager& operator = (const SoundEffectManager& rhs);

    struct pImpl;
    //const std::auto_ptr<pImpl> pimpl;
    pImpl* pimpl;

  public:
    enum {
      AllChannel = -1,
      InvalidId = 0,
      DefaultFadeOut = 100,
    };
    SoundEffectManager(void);
    ~SoundEffectManager(void);
    const char* what(void);

    bool isInitialized(void);
    bool registerEffect(int effect_id, const char* file_path);
    void unregisterEffect(int effect_id);

    // 音量の割合を設定
    void updateVolume(size_t percent, int channel_id = AllChannel);

    // 再生開始, 管理用の再生 ID を返す, 音量も指定可能, フェードイン指定
    int play(int effect_id, size_t fade_in_msec = 0, int volume_percent = 100);

    // ID 指定による停止、フェードアウト指定、終了を待つのも可
    void stop(int serial_id = AllChannel,
              size_t fade_out_msec = DefaultFadeOut);

    bool nowPlaying(int serial_id = AllChannel);

    // !!! 解放まわりのメソッド、そのうち

    // !!! 資源毎にボリュームを設定するメソッドとかか？
    void setEachVolume(size_t effect_id, size_t percent);

    //void terminate(void);
  };
};

#endif /* !SOUND_EFFECT_MANAGER_H */
