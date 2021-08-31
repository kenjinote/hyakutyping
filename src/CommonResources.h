#ifndef COMMON_RESOURCES_H
#define COMMON_RESOURCES_H

/*!
  \file
  \brief 共通リソースの管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingTextConverter.h"
#include "Component.h"
#include <string>


namespace beego {
  class GuiManager;
  class Layer;
  class TtfResource;
  class InputHandler;
  class InputReceiveComponent;
  class ConsolePanelComponent;
};
class TypingRecorder;
class AccessSettings;
class VoicePlayer;


/*!
  \brief 共通リソース管理
*/
class CommonResources {
  CommonResources(void);
  CommonResources(const char* savedir);
  CommonResources(const CommonResources& rhs);
  CommonResources& operator = (const CommonResources& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  //////////////////////////////////////////////////////////////////////
  // GUI 資源
  beego::GuiManager* gui_;       //!< GUI 管理
  beego::Layer* front_layer_;    //!< 前景レイヤー
  beego::Layer* back_layer_;     //!< 背景レイヤー
  boost::shared_ptr<beego::ConsolePanelComponent> debug_; //!< デバッグ表示用

  beego::Component back_label_;  //!< 背景画像
  beego::TtfResource* font_;     //!< フォントリソース
  beego::Component menu_icon_;   //!< メニューアイコン
  beego::Component user_label_;  //!< ユーザ名

  beego::InputHandler* input_;   //!< 入力ハンドラ
  boost::shared_ptr<beego::InputReceiveComponent> input_receiver_; //!< 入力受信

  //////////////////////////////////////////////////////////////////////
  // システム
  TypingRecorder* recorder_;     //!< タイピング情報の記録
  std::vector<int> waka_order_;  //!< 和歌の並び
  VoicePlayer* voice_;           //!< 詠み上げ

  //////////////////////////////////////////////////////////////////////
  // 設定
  std::string savedir_;             //!< データの保存ディレクトリ
  AccessSettings* settings_;        //!< 設定

  ~CommonResources(void);

  /*!
    \brief 共通リソースへの管理オブジェクトを返す
  */
  static CommonResources* getObject(const char* savedir = NULL);

  /*!
    \brief GUI リソースの初期化

    \attention スクリーンが生成されてから呼び出さなければならない
  */
  void initializeGui(void);

  /*!
    \brief フォントデータがあるかを返す

    \retval true フォントデータあり
    \retval false フォントデータなし
  */
  static bool accessFontData(void);

  /*!
    \brief 音楽データがあるかを返す

    \retval true 音楽データあり
    \retval false 音楽データなし
  */
  static bool accessMusicData(void);

  /*!
    \brief 音楽資源の初期化
  */
  void initializeMusic(void);

  /*!
    \brief 現在のユーザ名の取得
  */
  const char* getUserName(void) const;
  int getUserIndex(void);

  const char* getPassword(void) const;

  void changeUserIndex(size_t index);

  /*!
    \brief ユーザの削除

    \brief user_name [i] 追加ユーザ名
  */
  void addUser(const char* user_name, const char* password,
               const char* address);

  /*!
    \brief 指定ユーザの削除

    \brief user_name [i] 削除ユーザ番号
  */
  void deleteUser(int user_index);

  void updateUserLable(void);

  TypingTextConverter::ConvertType getConvertType(void);

  int getPracticeNum(void);

  int getPracticeNumIndex(void);
  void setPracticeNumIndex(size_t index);

  void setNextMusic(int id);
  bool nowSoundPlaying(void);
  void stopMusic(int fadeout_msec = 0);
  void haltMusic(void);
  void updateSoundVolume(int percent);

  bool registerEffect(int id, const char* file);
  int playEffect(int id);
  bool nowEffectPlaying(void);
  void stopEffect(int id = -1);
  void updateEffectVolume(int percent);

  std::string getServerName(void);
  std::string getServerAddress(void);

  int getPracticeWakaNo(void);
  bool isSendData(void);

  std::vector<std::string> getAllVoiceResources(void);
  std::string getVoiceResource(void);
};

#endif /* !COMMON_RESOURCES_H */
