#ifndef ACCESS_SETTINGS_H
#define ACCESS_SETTINGS_H

/*!
  \file
  \brief 設定の管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <vector>
#include <string>


/*!
  \brief 設定ファイルの管理クラス
*/
class AccessSettings {
  AccessSettings(void);
  AccessSettings(const AccessSettings& rhs);
  AccessSettings& operator = (const AccessSettings& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  std::vector<std::string> user_names_; //!< ユーザ名
  std::vector<std::string> user_password_; //!< パスワード
  std::vector<std::string> server_address_; //!< サーバアドレス

  std::vector<int> convert_mode_; //!< 入力モード
  std::vector<size_t> practice_num_; //!< 練習数

  std::vector<int> practice_mode_; //!< 練習モード
  std::vector<int> data_send_;     //!< データ送信

  std::vector<int> music_volume_; //!< 曲のボリューム
  std::vector<int> effect_volume_; //!< 効果音のボリューム
  std::vector<std::string> voice_resource_; //!< 詠み上げリソース

  size_t user_index_;           //!< 選択中のユーザインデックス

  std::string effect_decide_;   //!< 決定音
  std::string effect_type_;     //!< タイプ音
  std::string effect_miss_;     //!< ミスタイプ音

  AccessSettings(const char* savedir);
  ~AccessSettings(void);

  /*!
    \brief 再読み込み
  */
  void reload(void);

  /*!
    \brief 保存
  */
  void save(void);

  void deleteUser(int user_index);
};

#endif /* !ACCESS_SETTINGS_H */
