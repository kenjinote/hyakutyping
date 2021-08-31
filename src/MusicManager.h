#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

/*!
  \file
  \brief 背景曲の管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief 背景曲の管理クラス
  */
  class MusicManager {
    MusicManager(const MusicManager& rhs);
    MusicManager& operator = (const MusicManager& rhs);

    struct pImpl;
    //const std::auto_ptr<pImpl> pimpl;
    // !!! shared_ptr にすべきなのか？
    pImpl* pimpl;

  public:
    enum {
      Infinity = -1,                //!< 無限回の再生
    };
    MusicManager(void);
    ~MusicManager(void);

    /*!
      \brief 初期化済みかを返す

      \retval true 初期化済み
      \retval false 未初期化
    */
    bool isInitialized(void);

    /*!
      \brief 音楽の登録

      \param music_id [i] 登録したい ID 番号
      \param file_path [i] 音楽ファイルのあるパス

      \retval true 登録成功
      \retval false 登録失敗

      \attention 同じ music_id が複数指定された場合、最後の指定が有効になる
    */

    bool registerMusic(int music_id, const char* file_path);

    /*!
      \brief 音楽の登録削除

      \param music_id [i] 削除したい ID 番号
    */
    void unregisterMusic(int music_id);

    /*!
      \brief 音量の割合を設定

      \param percent [i] 音楽の出力ボリューム設置 [%]
    */
    void updateVolume(size_t percent);

    /*!
      \brief 曲の切り替え

      \param next_music_id [i] 次に再生させる音楽の ID 番号
      \param play_times [i] 再生回数 (0 で再生停止)
    */
    void setNextMusic(int next_music_id, int play_times = Infinity);

    /*!
      \brief 曲の停止

      \param fade_out_msec [i] フェードアウト時間 [msec]
    */
    void stop(size_t fade_out_msec = 0);

    void halt(void);

    /*!
      \brief フェードイン設定

      setNextMusic() 時に利用される

      \param effect_msec [i] フェードイン時間 [msec]
    */
    void setFadeInEffect(size_t effect_msec);

    /*!
      \brief フェードアウト設定

      setNextMusic(), stop() 時に利用される

      \param effect_msec [i] フェードアウト時間 [msec]
    */
    void setFadeOutEffect(size_t effect_msec);

    /*!
      \brief 現在再生中の音楽番号を指定

      \return 再生中の音楽番号

      \attention 再生中出ない場合、戻り値は無効
    */
    int getCurrentMusicId(void);

    /*!
      \brief 再生中かを返す

      \retval true 再生中
      \retval false 停止中
    */
    bool nowPlaying(void);
  };
};

#endif /* !MUSIC_MANAGER_H */
