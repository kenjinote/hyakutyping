/*!
  \file
  \brief 設定の管理

  \author Satofumi KAMIMURA

  $Id$

  \todo バックアップファイルを生成する
*/

#include "AccessSettings.h"
#include "ResourceDefinition.h"
#include "AccessProperty.h"
#include "SystemDefinition.h"
#include "hyakutypingrc_orig.h"
#include "ExistFile.h"
#include "CreateDirectory.h"
#include "CreateFileFromArray.h"
#include <boost/lexical_cast.hpp>
#include <cstring>

using namespace beego;
using namespace boost;


struct AccessSettings::pImpl {
  AccessProperty* property_;

  pImpl(const char* savedir) : property_(NULL) {

    // データ保存用ディレクトリを生成する
    if (! createDirectory(savedir)) {
      // !!! エラーメッセージの出力
      return;
    }

    // 設定ファイルがなければ、コピーして生成する
    std::string config_file = std::string(savedir) + ConfigFile;
    if (! existFile(config_file.c_str())) {
      createFileFromArray(config_file.c_str(), hyakutypingrc_orig,
                          sizeof(hyakutypingrc_orig) - 1);
    }
    property_ = new AccessProperty(config_file.c_str());
  }

  ~pImpl(void) {
    delete property_;
  }

  void swapUserSettings(int user_index, int remove_index) {

    // !!! initializeUserSettings と同じにすべき
    const char* swap_tag[] = {
      "u_%02d", "u_%02d_http", "u_%02d_password", "u_%02d_converter",
      "u_%02d_num", "u_%02d_music", "u_%02d_effect", "u_%02d_voice",
      "u_%02d_practice", "u_%02d_send",
    };

    enum { BufferMax = 256 };
    char swap_buffer0[BufferMax];
    char swap_buffer1[BufferMax];

    AccessProperty& property = *property_;
    for (size_t i = 0; i < sizeof(swap_tag)/sizeof(swap_tag[0]); ++i) {
      sprintf(swap_buffer0, swap_tag[i], user_index);
      sprintf(swap_buffer1, swap_tag[i], remove_index);

      std::swap(property[swap_buffer0], property[swap_buffer1]);
    }
  }

  void initializeUserSettings(int remove_index) {

    const char* tag[] = {
      "u_%02d", "u_%02d_http", "u_%02d_password", "u_%02d_converter",
      "u_%02d_num", "u_%02d_music", "u_%02d_effect", "u_%02d_voice",
      "u_%02d_practice", "u_%02d_send",
    };

    enum { BufferMax = 256 };
    char buffer[BufferMax];

    AccessProperty& property = *property_;
    for (size_t i = 0; i < sizeof(tag)/sizeof(tag[0]); ++i) {
      sprintf(buffer, tag[i], remove_index);
      property[buffer] = "";
    }
  }
};


AccessSettings::AccessSettings(const char* savedir)
  : pimpl(new pImpl(savedir)), user_index_(0) {
  reload();
}


AccessSettings::~AccessSettings(void) {
}


void AccessSettings::reload(void) {

  if (! pimpl->property_) {
    return;
  }

  AccessProperty& property = *pimpl->property_;

  // システム設定の読み出し
  effect_decide_ = property["effect_decide"];
  effect_type_ = property["effect_type"];
  effect_miss_ = property["effect_miss"];

  // ユーザ毎の初期化
  user_names_.resize(UserMax);
  user_password_.resize(UserMax);
  server_address_.resize(UserMax);
  practice_num_.resize(UserMax);
  convert_mode_.resize(UserMax);
  practice_mode_.resize(UserMax);
  data_send_.resize(UserMax);
  music_volume_.resize(UserMax);
  effect_volume_.resize(UserMax);
  voice_resource_.resize(UserMax);
  for (int i = 0; i < UserMax; ++i) {

    // !!! さすがに、このあたりの処理は関数化しますか...

    // ユーザ名
    char buffer[] = "u_xx";
    sprintf(buffer, "u_%02d", i);
    user_names_[i] = property[buffer];

    // パスワード
    char password_tag[] = "u_xx_password";
    sprintf(password_tag, "u_%02d_password", i);
    user_password_[i] = property[password_tag];

    // 登録アドレス
    char http_tag[] = "u_xx_http";
    sprintf(http_tag, "u_%02d_http", i);
    server_address_[i] = property[http_tag];

    // 練習数
    char num_tag[] = "u_xx_num";
    sprintf(num_tag, "u_%02d_num", i);
    size_t num = atoi(property[num_tag].c_str());
    if (num <= 0) {
      num = 5;
    }
    practice_num_[i] = num;

    // 入力モード
    char converter_tag[] = "u_xx_converter";
    sprintf(converter_tag, "u_%02d_converter", i);
    convert_mode_[i] = atoi(property[converter_tag].c_str());

    // 練習モード
    char practice_tag[] = "u_xx_practice";
    sprintf(practice_tag, "u_%02d_practice", i);
    practice_mode_[i] = atoi(property[practice_tag].c_str());

    // データ送信
    char send_tag[] = "u_xx_send";
    sprintf(send_tag, "u_%02d_send", i);
    data_send_[i] = atoi(property[send_tag].c_str());

    // 曲の音量
    char music_tag[] = "u_xx_music";
    sprintf(music_tag, "u_%02d_music", i);
    int volume = atoi(property[music_tag].c_str());
    music_volume_[i] = volume;

    // 効果音の音量
    char effect_tag[] = "u_xx_effect";
    sprintf(effect_tag, "u_%02d_effect", i);
    volume = atoi(property[effect_tag].c_str());
    effect_volume_[i] = volume;

    // 詠み上げリソース
    char voice_tag[] = "u_xx_voice";
    sprintf(voice_tag, "u_%02d_voice", i);
    voice_resource_[i] = property[voice_tag];
  }
  user_index_ = atoi(property["u_index"].c_str());
}


void AccessSettings::save(void) {

  // !!! isChangedByOthre() を用いて、上書きしてもよいか、の確認メッセージを出力すべき
  // !!! ま、後回し

  if (! pimpl->property_) {
    return;
  }

  AccessProperty& property = *pimpl->property_;

  // ユーザインデックスの記録
  property["u_index"] = lexical_cast<std::string>(user_index_);

  // ユーザ名、パスワードの記録
  for (int i = 0; i < UserMax; ++i) {

    // !!! 以降も関数化しましょう
    // !!! 余力あらば、タグまわりは load, save で共通にできるようにする

    // ユーザ名
    char name_tag[] = "u_xx";
    sprintf(name_tag, "u_%02d", i);
    property[name_tag] = user_names_[i];

    // パスワード
    char password_tag[] = "u_xx_password";
    sprintf(password_tag, "u_%02d_password", i);
    property[password_tag] = user_password_[i];

    // 登録アドレス
    char http_tag[] = "u_xx_http";
    sprintf(http_tag, "u_%02d_http", i);
    property[http_tag] = server_address_[i];

    // 練習数
    char num_tag[] = "u_xx_num";
    sprintf(num_tag, "u_%02d_num", i);
    property[num_tag] = lexical_cast<std::string>(practice_num_[i]);

    // 入力モード
    char converter_tag[] = "u_xx_converter";
    sprintf(converter_tag, "u_%02d_converter", i);
    property[converter_tag] = lexical_cast<std::string>(convert_mode_[i]);

    // 練習モード
    char practice_tag[] = "u_xx_practice";
    sprintf(practice_tag, "u_%02d_practice", i);
    property[practice_tag] = lexical_cast<std::string>(practice_mode_[i]);

    // データ送信
    char send_tag[] = "u_xx_send";
    sprintf(send_tag, "u_%02d_send", i);
    property[send_tag] = lexical_cast<std::string>(data_send_[i]);

    // 曲の音量
    char music_tag[] = "u_xx_music";
    sprintf(music_tag, "u_%02d_music", i);
    property[music_tag] = lexical_cast<std::string>(music_volume_[i]);

    // 効果音の音量
    char effect_tag[] = "u_xx_effect";
    sprintf(effect_tag, "u_%02d_effect", i);
    property[effect_tag] = lexical_cast<std::string>(effect_volume_[i]);

    // 詠み上げリソース
    char voice_tag[] = "u_xx_voice";
    sprintf(voice_tag, "u_%02d_voice", i);
    property[voice_tag] = voice_resource_[i];
  }

  property.save();
}


void AccessSettings::deleteUser(int index) {

  if (index < 0) {
    return;
  }

  // メンバ変数の初期化
  // !!! 格好悪いな...。いずれ、実装を見直して作り直す
  user_names_[index] = "";
  user_password_[index] = "";

  int remove_index = index;
  for (; remove_index < (UserMax - 1); ++remove_index) {
    if (strlen(user_names_[remove_index].c_str()) <= 0) {
      break;
    }
  }
  pimpl->initializeUserSettings(remove_index);
  pimpl->swapUserSettings(index, remove_index);

  if (static_cast<int>(user_index_) >= index) {
    --user_index_;
  }

  save();
}
