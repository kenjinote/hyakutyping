/*!
  \file
  \brief 和歌の描画

  \author Satofumi KAMIMURA

  $Id$

  \todo カーソルを表示
*/

#include "WakaDrawer.h"
#include "WakaData.h"
#include "DrawsDefinition.h"
#include "SystemDefinition.h"
#include "CommonResources.h"
#include "bs_bmp.h"
#include "return_bmp.h"
#include "CreateSurfaceFromArray.h"
#include "GuiManager.h"
#include "Layer.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "TextFadeSurface.h"
#include "TextRubiSurface.h"
#include "SdlSurface.h"
#include "AnimationSurface.h"
#include "LabelComponent.h"
#include "SdlUtils.h"
#include "GetTicks.h"
#include <string>

using namespace beego;


struct WakaDrawer::pImpl {
  enum {
    Base_Y = 40,
    Offset_Y = 30,
  };
  TypingMode typing_mode_;
  CommonResources* common_;
  size_t index_;
  std::vector<boost::shared_ptr<TextFadeSurface> > ku_surface_;
  std::vector<boost::shared_ptr<LabelComponent> > ku_label_;
  std::vector<boost::shared_ptr<TextRubiSurface> > rubi_surface_;
  std::vector<boost::shared_ptr<LabelComponent> > rubi_label_;
  boost::shared_ptr<TextSurface> roman_surface_;
  Component roman_label_;
  std::vector<boost::shared_ptr<TextSurface> > kana_surface_;
  std::vector<boost::shared_ptr<LabelComponent> > kana_label_;
  std::vector<boost::shared_ptr<TextSurface> > kimariji_kana_surface_;
  std::vector<boost::shared_ptr<LabelComponent> > kimariji_kana_label_;
  boost::shared_ptr<TextFadeSurface> yomibito_surface_;
  boost::shared_ptr<TextRubiSurface> yomibito_rubi_surface_;
  Component yomibito_label_;
  Component yomibito_rubi_label_;
  Surface bs_surface_;
  boost::shared_ptr<AnimationSurface> bs_animation_;
  Component bs_label_;
  Surface return_surface_;
  Component return_label_;
  bool draw_bs_;
  size_t first_msec_;
  bool input_sample_draw_[2];
  size_t ku_index_;
  bool sample_placed_;

  pImpl(TypingMode mode)
    : typing_mode_(mode), common_(CommonResources::getObject()),
      index_(0),
      bs_surface_(new SdlSurface(createSurface(bs_bmp, bs_bmp_width,
                                               bs_bmp_height), true)),
      bs_animation_(new AnimationSurface),
      return_surface_(new SdlSurface(createSurface(return_bmp, return_bmp_width,
                                                   return_bmp_height), true)),
      return_label_(new LabelComponent(return_surface_)),
      draw_bs_(false), ku_index_(0), sample_placed_(false) {

    ku_surface_.resize(5);
    ku_label_.resize(5);
    rubi_surface_.resize(5);
    rubi_label_.resize(5);
    kana_surface_.resize(2);
    kana_label_.resize(2);
    kimariji_kana_surface_.resize(2);
    kimariji_kana_label_.resize(2);

    bs_animation_->registerSurface(bs_surface_, 100, 0, 0);
    bs_animation_->registerSurface(bs_surface_, 100, 0, 2);
    bs_animation_->registerSurface(bs_surface_, 100, 2, 0);
    bs_animation_->registerSurface(bs_surface_, 100, 2, 2);
    bs_animation_->play();

    Component new_bs_label(new LabelComponent(bs_animation_));
    std::swap(bs_label_, new_bs_label);
  }

  void createWakaResource(size_t ku_index) {

    // 和歌サーフェスの生成
    TextProperty waka_property(common_->font_, KanjiWaka[index_][ku_index],
                               WakaDrawSize, Fore, Back, true);
    boost::shared_ptr<TextFadeSurface>
      new_ku_surface(new TextFadeSurface(waka_property));
    std::swap(ku_surface_[ku_index], new_ku_surface);
    ku_surface_[ku_index]->setFadePercent(0);

    boost::shared_ptr<LabelComponent>
      new_ku_label(new LabelComponent(ku_surface_[ku_index]));
    std::swap(ku_label_[ku_index], new_ku_label);

    // ルビサーフェスの生成
    TextProperty rubi_property(common_->font_, KanjiWaka[index_][ku_index],
                               WakaDrawSize, Fore, Back, true);
    boost::shared_ptr<TextRubiSurface>
      new_rubi_surface(new TextRubiSurface(KanaWaka[index_][ku_index],
                                           RubiDrawSize, rubi_property));
    std::swap(rubi_surface_[ku_index], new_rubi_surface);
    rubi_surface_[ku_index]->setFadePercent(0);

    boost::shared_ptr<LabelComponent>
      new_rubi_label(new LabelComponent(rubi_surface_[ku_index]));
    std::swap(rubi_label_[ku_index], new_rubi_label);
  }

  void createKanaResource(const std::vector<std::vector<Uint16> >& text) {

    for (int i = 0; i < 2; ++i) {
      TextProperty kana_property(common_->font_, &text[i + 3][0],
                                 WakaDrawSize, Fore, Back, true);
      boost::shared_ptr<TextSurface>
        new_kana_surface(new TextSurface(kana_property));
      std::swap(kana_surface_[i], new_kana_surface);
      kana_surface_[i]->setAlpha(0);

      boost::shared_ptr<LabelComponent>
        new_kana_label(new LabelComponent(kana_surface_[i]));
      std::swap(kana_label_[i], new_kana_label);

      SDL_Rect position;
      set_SdlRect(&position, (640 - InputWidth)/2 + TextOffset,
                  (480 - InputSize - BottomOffset - SampleOffset));
      kana_label_[i]->setPosition(&position);

      if (typing_mode_ == KimarijiTyping) {
        // 決まり字用に最初の２文字を別の色で作る
        std::vector<Uint16> color_text = text[i + 3];
        color_text[2] = 0x0;

        TextProperty kimariji_property(common_->font_, &color_text[0],
                                       WakaDrawSize, Selected, Back, true);
        boost::shared_ptr<TextSurface>
          new_kimariji_surface(new TextSurface(kimariji_property));
        std::swap(kimariji_kana_surface_[i], new_kimariji_surface);
        kimariji_kana_surface_[i]->setAlpha(0);

        boost::shared_ptr<LabelComponent>
          new_kimariji_label(new LabelComponent(kimariji_kana_surface_[i]));
        std::swap(kimariji_kana_label_[i], new_kimariji_label);
        kimariji_kana_label_[i]->setPosition(&position);
      }
    }
  }

  void createRomanResource(const std::vector<Uint16>& text) {

    TextProperty roman_property(common_->font_, &text[0],
                                RomanSize, Fore, Back, true);
    boost::shared_ptr<TextSurface>
      new_roman_surface(new TextSurface(roman_property));
    std::swap(roman_surface_, new_roman_surface);
    if (! input_sample_draw_[ku_index_]) {
      roman_surface_->setAlpha(0);
    }

    Component new_roman_label(new LabelComponent(roman_surface_));
    std::swap(roman_label_, new_roman_label);
  }

  void createYomibitoResource(void) {

    // 詠み人(漢字)
    TextProperty yomibito_property(common_->font_,
                                   YomibitoName[index_][0],
                                   YomibitoDrawSize, Fore, Back, true);
    boost::shared_ptr<TextFadeSurface>
      new_surface(new TextFadeSurface(yomibito_property));
    std::swap(yomibito_surface_, new_surface);
    yomibito_surface_->setFadePercent(0);

    Component new_label(new LabelComponent(yomibito_surface_));
    std::swap(yomibito_label_, new_label);

    // 詠み人(るび)
    boost::shared_ptr<TextRubiSurface>
      new_rubi_surface(new TextRubiSurface(YomibitoName[index_][1],
                                           YomibitoRubiDrawSize,
                                           yomibito_property));
    std::swap(yomibito_rubi_surface_, new_rubi_surface);
    yomibito_rubi_surface_->setFadePercent(0);

    Component new_rubi_label(new LabelComponent(yomibito_rubi_surface_));
    std::swap(yomibito_rubi_label_, new_rubi_label);
  }

  void placeInputSampleComponents(void) {
    if (sample_placed_ != false) {
      return;
    }
    sample_placed_ = true;

    placeRomanResource();
    common_->front_layer_->push_front(kana_label_[ku_index_]);
    if (typing_mode_ == KimarijiTyping) {
      common_->front_layer_->push_front(kimariji_kana_label_[ku_index_]);
    }
  }

  void removeInputSampleComponents(void) {
    if (sample_placed_ == false) {
      return;
    }
    sample_placed_ = false;

    for (size_t i = 0; i < 2; ++i) {
      common_->front_layer_->remove(kana_label_[i]);
      if (typing_mode_ == KimarijiTyping) {
        common_->front_layer_->remove(kimariji_kana_label_[i]);
      }
    }
    common_->front_layer_->remove(roman_label_);
    common_->front_layer_->remove(bs_label_);
    common_->front_layer_->remove(return_label_);
    SDL_Rect position;
    return_label_->getPosition(&position);
  }

  void updateWakaPosition(void) {
    enum {
      SpaceSize = WakaDrawSize * 4/5,
    };
    int length_1st = SpaceSize +
      static_cast<int>(ku_label_[0]->getWidth() + ku_label_[1]->getWidth());
    int length_3rd = WakaDrawSize + SpaceSize +
      static_cast<int>(ku_label_[3]->getWidth() + ku_label_[4]->getWidth());
    int length_max = (length_1st > length_3rd) ? length_1st : length_3rd;

    // 最大の行の長さを基準に base_x, base_y を計算する
    int base_x = (640 - length_max) / 2;
    SDL_Rect position[5];
    set_SdlRect(&position[0], base_x, Base_Y);
    set_SdlRect(&position[1], base_x +
                static_cast<int>(ku_label_[0]->getWidth()) + SpaceSize, Base_Y);
    set_SdlRect(&position[2],
                base_x + WakaDrawSize + SpaceSize,
                Base_Y + WakaDrawSize + Offset_Y);
    set_SdlRect(&position[3],
                base_x + WakaDrawSize, Base_Y + (WakaDrawSize + Offset_Y) * 2);
    set_SdlRect(&position[4],
                base_x + WakaDrawSize + SpaceSize +
                static_cast<int>(ku_label_[3]->getWidth()),
                Base_Y + (WakaDrawSize + Offset_Y) * 2);

    for (size_t i = 0; i < 5; ++i) {
      ku_label_[i]->setPosition(&position[i]);

      SDL_Rect rubi_position = position[i];
      rubi_position.y -= RubiDrawSize;
      rubi_position.x += rubi_surface_[i]->getDrawOffset();
      rubi_label_[i]->setPosition(&rubi_position);
    }
    SDL_Rect yomibito_position;
    set_SdlRect(&yomibito_position, 640 -
                static_cast<int>(yomibito_label_->getWidth()) - WakaDrawSize,
                Base_Y + (WakaDrawSize + Offset_Y) * 3);
    yomibito_label_->setPosition(&yomibito_position);

    yomibito_position.y -= RubiDrawSize;
    yomibito_position.x += yomibito_rubi_surface_->getDrawOffset();
    yomibito_rubi_label_->setPosition(&yomibito_position);
  }

  void placeRomanResource(void) {

    SDL_Rect position;
    set_SdlRect(&position, centerPosition(roman_label_, 640/2),
                480 - InputSize * 2 - BottomOffset - TextOffset - RomanOffset);
    roman_label_->setPosition(&position);
    if (roman_label_->getWidth() > 0) {
      common_->front_layer_->push_front(roman_label_);
    }

    position.y += TextOffset;
    if (draw_bs_) {
      position.x =
        640/2 - static_cast<int>(roman_label_->getWidth()/2) - RomanSize/2 - 4;
      bs_label_->setPosition(&position);
      common_->front_layer_->push_front(bs_label_);
    }

    position.x = 640/2 + static_cast<int>(roman_label_->getWidth()/2) + 4;
    return_label_->setPosition(&position);
    common_->front_layer_->push_front(return_label_);
    return_label_->getPosition(&position);
  }

  void updateWakaAlpha(size_t kaminoku_percent, size_t shimonoku_percent,
                       size_t yomibito_percent) {

    // 和歌の表示
    size_t ku[5];
    ku[0] = kaminoku_percent * 3;
    ku[1] = (kaminoku_percent > 33) ? (kaminoku_percent - 33) * 3 : 0;
    ku[2] = (kaminoku_percent > 66) ? (kaminoku_percent - 66) * 3 : 0;
    ku[3] = shimonoku_percent * 2;
    ku[4] = (shimonoku_percent > 50) ? (shimonoku_percent - 50) * 2 : 0;

    for (int i = 0; i < 5; ++i) {
      ku_surface_[i]->setFadePercent(ku[i]);
      rubi_surface_[i]->setFadePercent(ku[i]);
    }

    // 詠み人の表示
    yomibito_surface_->setFadePercent(yomibito_percent);
    yomibito_rubi_surface_->setFadePercent(yomibito_percent);
  }
};


WakaDrawer::WakaDrawer(TypingMode mode) : pimpl(new pImpl(mode)) {
}


WakaDrawer::~WakaDrawer(void) {
}


void WakaDrawer::setWakaIndex(size_t index) {

  removeComponents();

  pimpl->index_ = index;

  for (size_t i = 0; i < 5; ++i) {
    pimpl->createWakaResource(i);
  }
  pimpl->createYomibitoResource();

  // フラグの初期化
  for (size_t i = 0; i < 2; ++i) {
    pimpl->input_sample_draw_[i] = false;
  }

  // サーフェスの配置
  pimpl->updateWakaPosition();
  placeComponents();
}


void WakaDrawer::setInputSample(const std::vector<std::vector<Uint16> >& text,
                                const std::vector<Uint16>& roman_sample) {

  // サーフェスの作成
  pimpl->createRomanResource(roman_sample);
  pimpl->createKanaResource(text);

  pimpl->first_msec_ = GetTicks();
}


void WakaDrawer::updateInputSample(size_t ku_index,
                                   const std::vector<Uint16>& roman_sample,
                                   bool need_bs) {
  pimpl->draw_bs_ = need_bs;

  // 登録の削除と登録を行うことで、コンポーネントの再描画を行う

  // 登録削除
  pimpl->removeInputSampleComponents();

  // サーフェスの更新
  pimpl->ku_index_ = ku_index;
  pimpl->createRomanResource(roman_sample);

  // 再登録
  pimpl->placeInputSampleComponents();
}


void WakaDrawer::draw(void) {

  // 描画済みのサーフェスに対し、表示率を更新する
  size_t spent_msec = GetTicks() - pimpl->first_msec_;

  // 和歌の描画
  size_t kaminoku_percent = 0;
  size_t shimonoku_percent = 0;
  size_t yomibito_percent = 0;
  if (spent_msec < KaminokuPlaying) {
    // 上の句の描画数値を更新
    kaminoku_percent = 100 * spent_msec / KaminokuPlaying;

  } else if (spent_msec < TotalPlaying) {
    // 下の句の描画数値を更新
    kaminoku_percent = 100;
    shimonoku_percent =
      100 * (spent_msec - KaminokuPlaying) / ShimonokuPlaying;
  } else {
    kaminoku_percent = 100;
    shimonoku_percent = 100;
    yomibito_percent = 100 * (spent_msec - TotalPlaying) / YomibitoPlaying;
  }
  pimpl->updateWakaAlpha(kaminoku_percent,
                         shimonoku_percent, yomibito_percent);

  if (spent_msec < KaminokuPlaying/3) {
    // !!! 読み上げの処理(１番目の句)
    // !!!

  } else if (spent_msec < KaminokuPlaying/3*2) {
    // !!! 読み上げの処理(２番目の句)

  } else if (spent_msec < KaminokuPlaying) {
    // !!! 読み上げの処理(３番目の句)

  } else if (spent_msec < KaminokuPlaying + ShimonokuPlaying/2) {

    // !!! 読み上げの処理(４番目の句)

    // !!! 以降は、少なくとも関数化すべき
    if (pimpl->ku_index_ == 0) {
      // 入力候補の表示(下の句、１番目)
      if (! pimpl->input_sample_draw_[0]) {
        pimpl->roman_surface_->setAlpha(100);
        pimpl->kana_surface_[0]->setAlpha(100);
        if (pimpl->typing_mode_ == KimarijiTyping) {
          pimpl->kimariji_kana_surface_[0]->setAlpha(100);
        }
        pimpl->input_sample_draw_[0] = true;
      }
    } else {
      pimpl->roman_surface_->setAlpha(0);
    }
  } else {
    // !!! 読み上げの処理(５番目の句)

    if (pimpl->ku_index_ == 1) {
      // 入力候補の表示(下の句、２番目)
      if (! pimpl->input_sample_draw_[1]) {
        pimpl->roman_surface_->setAlpha(100);
        pimpl->kana_surface_[1]->setAlpha(100);
        if (pimpl->typing_mode_ == KimarijiTyping) {
          pimpl->kimariji_kana_surface_[1]->setAlpha(100);
        }
        pimpl->input_sample_draw_[1] = true;
      }
    }
  }
}


void WakaDrawer::placeComponents(void) {

  for (int i = 0; i < 5; ++i) {
    pimpl->common_->front_layer_->push_front(pimpl->ku_label_[i]);
    if (pimpl->rubi_label_[i]->getWidth() > 0) {
      // !!! るびがない場合、登録すると挙動がおかしくなるのに対処
      // !!! 本来は、TextRubiSurface 内で修正するべきだが、まぁ、とりあえず
      // !!! いずれ修正すること
      pimpl->common_->front_layer_->push_front(pimpl->rubi_label_[i]);
    }
  }
  pimpl->common_->front_layer_->push_front(pimpl->yomibito_label_);
  pimpl->common_->front_layer_->push_front(pimpl->yomibito_rubi_label_);
}


void WakaDrawer::removeComponents(void) {

  removeInputComponents();

  for (size_t i = 0; i < 5; ++i) {
    pimpl->common_->front_layer_->remove(pimpl->ku_label_[i]);
    pimpl->common_->front_layer_->remove(pimpl->rubi_label_[i]);
  }
  pimpl->common_->front_layer_->remove(pimpl->yomibito_label_);
  pimpl->common_->front_layer_->remove(pimpl->yomibito_rubi_label_);
}


void WakaDrawer::removeInputComponents(void) {
  pimpl->removeInputSampleComponents();
}
