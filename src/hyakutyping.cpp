/*!
  \file
  \brief 百人一首タイピング

  \author Satofumi KAMIMURA

  $Id$

  \todo -W 付きでコンパイルが通過するようにする

  \todo ボリュームは、どこからでも変更できるようにする
  \todo フルスクリーンのキーを反映
  \todo アニメーションアイコンのジャギーを修正する
  \todo マウスで戻ったときに、キー操作が上書きされてしまうのをなんとかする
  \todo 終了しますか？ Yes/No の項目をつける？
  \todo メニューの遷移直後のカーソルでメニュー項目に移動するのを、抑制する
  \todo TextProperty から LabelComponent を作るクラスも作るべきか？
  \todo Ctrl を押すと、可能なキー入力についての説明が表示されるとか？
  \todo ネットワーク対戦、の実装
  \todo 項目が変更される毎に、効果音を鳴らす
  \todo フルスクリーンのときは、マウスの移動がなければ隠す
  \todo lexical_cast の例外を拾うべき
  \todo 工事中のメニュー項目に、工事中アイコンを付加するとか
*/

#include "hyakutyping.h"
#include "hyakutyping_bmp.h"
#include "icon_mask.h"
#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "VoicePlayer.h"
#include "FirstMenu.h"
#include "CreateSurfaceFromArray.h"
#include "SdlVideo.h"

using namespace beego;
using namespace SDL;
//using namespace GL;


static std::string getSaveDirectory(void) {

  std::string savedir = SaveDir;
  if (! savedir.compare(0, 2, "~/")) {
    // "~/" から始まる場合、環境変数 HOME で置き換える
    const char* home = getenv("HOME");
    if (home != NULL) {
      savedir.replace(0, 1, home);
    }
  }

  return savedir;
}


/*!
  \brief メイン関数
*/
int main(int argc, char *argv[]) {

  // 引数の処理
  bool music_mute = false;

  // !!! 個人設定を読み出すように変更する
  // !!! 個人設定、引数設定、の順に評価を更新する
  bool fullscreen = false;
  for (int i = 1; i < argc; ++i) {
    if ((! strcmp("-m", argv[i])) || (! strcmp("--mute", argv[i]))) {
      // ミュート設定
      music_mute = true;

    } else if ((! strcmp("-fs", argv[i])) ||
               (! strcmp("--fullscreen", argv[i]))) {
      // スクリーンの最大化
      fullscreen = true;
    }
  }

  // 画面の初期化
  SdlVideo video;
  video.setCaption(PACKAGE_STRING);
  SDL_Surface* icon = createSurface(hyakutyping_bmp,
                                    hyakutyping_bmp_width,
                                    hyakutyping_bmp_height);
  video.setIcon(icon, icon_mask);
  video.setFullscreenMode(fullscreen);
  if (! video.show()) {
    printf("SdlVideo::show(): %s\n", video.what());
    exit(1);
  }

  // データの存在チェック
  std::string savedir = getSaveDirectory();
  CommonResources* common = CommonResources::getObject(savedir.c_str());
  if (! CommonResources::accessFontData()) {
    // フォントデータがなければ、終了する
    // !!! 可能なら、フォントデータも実行バイナリに含めたい
    exit(1);
  }
  if (! CommonResources::accessMusicData()) {
    music_mute = true;
  }

  // GUI 資源の初期化
  common->initializeGui();

  // 音楽資源の初期化
  if (! music_mute) {
    common->initializeMusic();
  } else {
    // 詠み上げも行わないようにする
    common->voice_->setMute();
  }

  // メニュー画面の呼び出し
  FirstMenu menu;
  menu.run();

  return 0;
}
