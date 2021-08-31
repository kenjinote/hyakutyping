#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

/*!
  \file
  \brief �w�i�Ȃ̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief �w�i�Ȃ̊Ǘ��N���X
  */
  class MusicManager {
    MusicManager(const MusicManager& rhs);
    MusicManager& operator = (const MusicManager& rhs);

    struct pImpl;
    //const std::auto_ptr<pImpl> pimpl;
    // !!! shared_ptr �ɂ��ׂ��Ȃ̂��H
    pImpl* pimpl;

  public:
    enum {
      Infinity = -1,                //!< ������̍Đ�
    };
    MusicManager(void);
    ~MusicManager(void);

    /*!
      \brief �������ς݂���Ԃ�

      \retval true �������ς�
      \retval false ��������
    */
    bool isInitialized(void);

    /*!
      \brief ���y�̓o�^

      \param music_id [i] �o�^������ ID �ԍ�
      \param file_path [i] ���y�t�@�C���̂���p�X

      \retval true �o�^����
      \retval false �o�^���s

      \attention ���� music_id �������w�肳�ꂽ�ꍇ�A�Ō�̎w�肪�L���ɂȂ�
    */

    bool registerMusic(int music_id, const char* file_path);

    /*!
      \brief ���y�̓o�^�폜

      \param music_id [i] �폜������ ID �ԍ�
    */
    void unregisterMusic(int music_id);

    /*!
      \brief ���ʂ̊�����ݒ�

      \param percent [i] ���y�̏o�̓{�����[���ݒu [%]
    */
    void updateVolume(size_t percent);

    /*!
      \brief �Ȃ̐؂�ւ�

      \param next_music_id [i] ���ɍĐ������鉹�y�� ID �ԍ�
      \param play_times [i] �Đ��� (0 �ōĐ���~)
    */
    void setNextMusic(int next_music_id, int play_times = Infinity);

    /*!
      \brief �Ȃ̒�~

      \param fade_out_msec [i] �t�F�[�h�A�E�g���� [msec]
    */
    void stop(size_t fade_out_msec = 0);

    void halt(void);

    /*!
      \brief �t�F�[�h�C���ݒ�

      setNextMusic() ���ɗ��p�����

      \param effect_msec [i] �t�F�[�h�C������ [msec]
    */
    void setFadeInEffect(size_t effect_msec);

    /*!
      \brief �t�F�[�h�A�E�g�ݒ�

      setNextMusic(), stop() ���ɗ��p�����

      \param effect_msec [i] �t�F�[�h�A�E�g���� [msec]
    */
    void setFadeOutEffect(size_t effect_msec);

    /*!
      \brief ���ݍĐ����̉��y�ԍ����w��

      \return �Đ����̉��y�ԍ�

      \attention �Đ����o�Ȃ��ꍇ�A�߂�l�͖���
    */
    int getCurrentMusicId(void);

    /*!
      \brief �Đ�������Ԃ�

      \retval true �Đ���
      \retval false ��~��
    */
    bool nowPlaying(void);
  };
};

#endif /* !MUSIC_MANAGER_H */
