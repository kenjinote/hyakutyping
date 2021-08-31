#ifndef SOUND_EFFECT_MANAGER_H
#define SOUND_EFFECT_MANAGER_H

/*!
  \file
  \brief ���ʉ��̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief ���ʉ��̊Ǘ��N���X
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

    // ���ʂ̊�����ݒ�
    void updateVolume(size_t percent, int channel_id = AllChannel);

    // �Đ��J�n, �Ǘ��p�̍Đ� ID ��Ԃ�, ���ʂ��w��\, �t�F�[�h�C���w��
    int play(int effect_id, size_t fade_in_msec = 0, int volume_percent = 100);

    // ID �w��ɂ���~�A�t�F�[�h�A�E�g�w��A�I����҂̂���
    void stop(int serial_id = AllChannel,
              size_t fade_out_msec = DefaultFadeOut);

    bool nowPlaying(int serial_id = AllChannel);

    // !!! ����܂��̃��\�b�h�A���̂���

    // !!! �������Ƀ{�����[����ݒ肷�郁�\�b�h�Ƃ����H
    void setEachVolume(size_t effect_id, size_t percent);

    //void terminate(void);
  };
};

#endif /* !SOUND_EFFECT_MANAGER_H */
