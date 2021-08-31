/*!
  \file
  \brief 共通リソースの管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include "CommonResources.h"
#include "ResourceDefinition.h"
#include "SystemDefinition.h"
#include "DrawsDefinition.h"
#include "AccessSettings.h"
#include "TypingRecorder.h"
#include "VoicePlayer.h"
#include "GuiManager.h"
#include "Layer.h"
#include "InputHandler.h"
#include "NullSurface.h"
#include "TextSurface.h"
#include "FillSurface.h"
#include "SdlSurface.h"
#include "AnimationSurface.h"
#include "TtfResource.h"
#include "TextProperty.h"
#include "ConsolePanelComponent.h"
#include "InputReceiveComponent.h"
#include "LabelComponent.h"
#include "SearchFilePath.h"
#include "back_bmp.h"
#include "selector_00_bmp.h"
#include "selector_01_bmp.h"
#include "selector_02_bmp.h"
#include "selector_03_bmp.h"
#include "CreateSurfaceFromArray.h"
#include "MusicManager.h"
#include "SoundEffectManager.h"
#include "ExistFile.h"
#include "SdlUtils.h"

using namespace beego;


struct CommonResources::pImpl {
  SDL_Surface* scr_;
  Layer debug_layer_;
  Surface back_bmp_surface_; // !!! 保持する必要があるんだっけ？ このあたりの資源
  Surface back_surface_;
  boost::shared_ptr<AnimationSurface> icon_surface_;
  bool music_initialized_;
  beego::MusicManager* music_;              //!< 背景曲の管理
  beego::SoundEffectManager* sound_effect_; //!< 効果音の管理

  pImpl(void)
    : scr_(SDL_GetVideoSurface()), music_initialized_(false),
      music_(NULL), sound_effect_(NULL) {
  }

  ~pImpl(void) {
    delete music_;
    delete sound_effect_;
  }
};


CommonResources::CommonResources(const char* savedir)
  : pimpl(new pImpl),
    gui_(new GuiManager), front_layer_(new Layer), back_layer_(new Layer),
    recorder_(new TypingRecorder),
    voice_(new VoicePlayer(VoiceDir)), savedir_(savedir),
    settings_(new AccessSettings(savedir)) {

  gui_->push_back(back_layer_);
  gui_->push_front(front_layer_);
  gui_->push_front(&pimpl->debug_layer_);
}


CommonResources::~CommonResources(void) {

  delete input_;
  delete font_;
  delete back_layer_;
  delete front_layer_;
  delete gui_;
}


CommonResources* CommonResources::getObject(const char* savedir) {
  static CommonResources singleton(savedir);
  return &singleton;
}


void CommonResources::initializeGui(void) {

  if (! pimpl->scr_) {
    pimpl->scr_ = SDL_GetVideoSurface();
  }

  // 背景画像の準備
  Surface new_back_bmp_surface(new SdlSurface(createSurface(back_bmp,
                                                            back_bmp_width,
                                                            back_bmp_height)));
  std::swap(pimpl->back_bmp_surface_, new_back_bmp_surface);

  Surface new_back_surface(new FillSurface(pimpl->back_bmp_surface_,
                                           pimpl->scr_->w, pimpl->scr_->h));
  std::swap(pimpl->back_surface_, new_back_surface);

  Component new_back_label(new LabelComponent(pimpl->back_surface_));
  std::swap(back_label_, new_back_label);

  // メニューアイコンの準備
  boost::shared_ptr<AnimationSurface> new_icon_surface(new AnimationSurface);
  Surface cell_00(new SdlSurface(createSurface(selector_00_bmp,
                                               selector_00_bmp_width,
                                               selector_00_bmp_height), true));
  Surface cell_01(new SdlSurface(createSurface(selector_01_bmp,
                                               selector_01_bmp_width,
                                               selector_01_bmp_height), true));
  Surface cell_02(new SdlSurface(createSurface(selector_02_bmp,
                                               selector_02_bmp_width,
                                               selector_02_bmp_height), true));
  Surface cell_03(new SdlSurface(createSurface(selector_03_bmp,
                                               selector_03_bmp_width,
                                               selector_03_bmp_height), true));
  new_icon_surface->registerSurface(cell_00, 230, 0, 0);
  new_icon_surface->registerSurface(cell_01, 200, 8, 0);
  new_icon_surface->registerSurface(cell_02, 200, 8, 0);
  new_icon_surface->registerSurface(cell_03, 230, 0, 0);
  new_icon_surface->registerSurface(cell_02, 200, 8, 0);
  new_icon_surface->registerSurface(cell_01, 200, 8, 0);
  new_icon_surface->play();
  std::swap(pimpl->icon_surface_, new_icon_surface);

  Component new_menu_icon(new LabelComponent(pimpl->icon_surface_));
  std::swap(menu_icon_, new_menu_icon);

  // 入力資源の準備
  font_ = new TtfResource(FontPath);
  input_ = new InputHandler;

  boost::shared_ptr<InputReceiveComponent>
    new_input_receiver(new InputReceiveComponent);
  std::swap(input_receiver_, new_input_receiver);

  // デバッグパネルの準備
  TextProperty text_property(font_, "", SmallSize, Black, Gray4, true);
  boost::shared_ptr<ConsolePanelComponent>
    panel(new ConsolePanelComponent(640, 480, text_property,
                                    ConsolePanelComponent::FadeOut,
                                    ConsolePanelComponent::Up));
  std::swap(debug_, panel);

  // コンポーネントの配置
  pimpl->debug_layer_.push_front(debug_);
  back_layer_->push_back(input_receiver_);
}


bool CommonResources::accessFontData(void) {

  //std::string font_path;
  //std::vector<std::string> dir_path;
  //if (! searchFilePath(font_path, FontPath, dir_path)) {
  if (! existFile(FontPath)) {
    printf("\"%s\" is not found.\n", FontPath);
    return false;
  }
  return true;
}


bool CommonResources::accessMusicData(void) {

  //if (existFile(Music1_FilePath) && existFile(Music2_FilePath)) {
  if (existFile(MusicDirectory)) {
    return true;
  } else {
    return false;
  }
}


void CommonResources::initializeMusic(void) {

  if (! pimpl->music_initialized_) {

    // オブジェクトの宣言
    pimpl->music_ = new MusicManager;
    if (! pimpl->music_->isInitialized()) {
      delete pimpl->music_;
      pimpl->music_ = NULL;
      return;
    }
    pimpl->sound_effect_ = new SoundEffectManager;

    // 再生効果
    pimpl->music_->setFadeInEffect(PlayFadeIn);

    // 音楽資源の初期化
    pimpl->music_->registerMusic(BackMusic_1, Music1_FilePath);
    pimpl->music_->registerMusic(BackMusic_2, Music2_FilePath);

    pimpl->sound_effect_->registerEffect(SoundEffect_1, Effect1_FilePath);
    pimpl->sound_effect_->registerEffect(SoundEffect_2, Effect2_FilePath);

#if 0
    // 音楽資源を固定にするとき
    pimpl->sound_effect_->registerEffect(Decide, Decide_FilePath);
    pimpl->sound_effect_->registerEffect(Type, Type_FilePath);
    pimpl->sound_effect_->registerEffect(Miss, Miss_FilePath);
#else
    // 音楽リソースを hyakutypingrc.txt から指定可能にする
    std::string dir_path = std::string(MusicDirectory);
    std::string decide_file = dir_path + settings_->effect_decide_;
    std::string type_file = dir_path + settings_->effect_type_;
    std::string miss_file = dir_path + settings_->effect_miss_;

    pimpl->sound_effect_->registerEffect(Decide, decide_file.c_str());
    pimpl->sound_effect_->registerEffect(Type, type_file.c_str());
    pimpl->sound_effect_->registerEffect(Miss, miss_file.c_str());
#endif

    // !!! さすがに、pImpl::user_index_ を宣言する？
    size_t user_index = settings_->user_index_;
    int volume = settings_->music_volume_[user_index];
    updateSoundVolume(volume);

    volume = settings_->effect_volume_[user_index];
    updateEffectVolume(volume);

    pimpl->music_initialized_ = true;
  }
}


const char* CommonResources::getUserName(void) const {

  size_t user_index = settings_->user_index_;
  return settings_->user_names_[user_index].c_str();
}


int CommonResources::getUserIndex(void) {

  // !!! この関数を使うように変更する？
  return settings_->user_index_;
}


const char* CommonResources::getPassword(void) const {

  size_t user_index = settings_->user_index_;
  return settings_->user_password_[user_index].c_str();
}


void CommonResources::changeUserIndex(size_t index) {
  settings_->user_index_ = index;
  settings_->save();
}


void CommonResources::addUser(const char* user_name, const char* password,
                              const char* address) {

  int index = 0;
  for (std::vector<std::string>::iterator it = settings_->user_names_.begin();
       it != settings_->user_names_.end(); ++it, ++index) {
    if (! it->compare("")) {
      *it = user_name;
      settings_->user_password_[index] = password;
      settings_->server_address_[index] = address;
      settings_->user_index_ = index;

      break;
    }
  }
}


void CommonResources::deleteUser(int user_index) {
  settings_->deleteUser(user_index);
}


void CommonResources::updateUserLable(void) {
  size_t user_index = settings_->user_index_;
  const char* user_name = settings_->user_names_[user_index].c_str();

  if (strlen(user_name) <= 0) {
    Surface new_user_surface(new NullSurface);
    Component new_user_label(new LabelComponent(new_user_surface));
    std::swap(user_label_, new_user_label);
  } else {
    TextProperty user_property(font_, user_name,
                               NormalSize, Fore, Back, true);
    Surface new_user_surface(new TextSurface(user_property));
    Component new_user_label(new LabelComponent(new_user_surface));
    std::swap(user_label_, new_user_label);
  }

  SDL_Rect position;
  set_SdlRect(&position,
              rightPosition(user_label_, 640) - TextOffset, TextOffset);
  user_label_->setPosition(&position);
}


TypingTextConverter::ConvertType CommonResources::getConvertType(void) {

  size_t user_index = settings_->user_index_;
  return (settings_->convert_mode_[user_index] == 0) ?
    TypingTextConverter::Roman : TypingTextConverter::Kana;
}


int CommonResources::getPracticeNum(void) {

  size_t user_index = settings_->user_index_;
  return settings_->practice_num_[user_index];
}


int CommonResources::getPracticeNumIndex(void) {

  int index = 0;
  switch (getPracticeNum()) {
  case 10:
    index = 1;
    break;

  case 50:
    index = 2;
    break;

  case 100:
    index = 3;
    break;
  }

  return index;
}


void CommonResources::setPracticeNumIndex(size_t index) {

  int num_array[] = { 5, 10, 50, 100 };
  if (index >= sizeof(num_array)/sizeof(num_array[0])) {
    return;
  }

  size_t user_index = settings_->user_index_;
  settings_->practice_num_[user_index] = num_array[index];
}


void CommonResources::setNextMusic(int id) {
  if (pimpl->music_) {
    pimpl->music_->setNextMusic(id);
  }
}


bool CommonResources::nowSoundPlaying(void) {
  if (! pimpl->music_) {
    return false;
  }

  return pimpl->music_->nowPlaying();
}


void CommonResources::stopMusic(int fadeout_msec) {
  if (pimpl->music_) {
    pimpl->music_->stop(fadeout_msec);
  }
}


void CommonResources::haltMusic(void) {
  if (pimpl->music_) {
    pimpl->music_->halt();
  }
}


void CommonResources::updateSoundVolume(int percent) {
  if (pimpl->music_) {
    pimpl->music_->updateVolume(percent);
  }
}


bool CommonResources::registerEffect(int id, const char* file) {
  if (pimpl->music_) {
    return pimpl->sound_effect_->registerEffect(id, file);
  }
  return false;
}


int CommonResources::playEffect(int id) {
  if (pimpl->music_) {
    return pimpl->sound_effect_->play(id);
  }
  return -1;
}


bool CommonResources::nowEffectPlaying(void) {
  if (! pimpl->music_) {
    return false;
  }
  return pimpl->sound_effect_->nowPlaying();
}


void CommonResources::stopEffect(int id) {
  if (pimpl->music_) {
    // !!! フェードアウト時間は、リソース定義に含めるべきかと
    pimpl->sound_effect_->stop(id, 1000);
  }
}


void CommonResources::updateEffectVolume(int percent) {
  if (pimpl->music_) {
    pimpl->sound_effect_->updateVolume(percent);
  }
}


std::string CommonResources::getServerName(void) {

  size_t user_index = settings_->user_index_;
  std::string server = settings_->server_address_[user_index];
  size_t last = server.find("/");

  return server.substr(0, last);
}


std::string CommonResources::getServerAddress(void) {

  size_t user_index = settings_->user_index_;
  std::string address = settings_->server_address_[user_index];
  size_t first = address.find("/");

  return address.substr(first, std::string::npos);
}


int CommonResources::getPracticeWakaNo(void) {

  size_t user_index = settings_->user_index_;
  return settings_->practice_mode_[user_index];
}


bool CommonResources::isSendData(void) {

  size_t user_index = settings_->user_index_;
  return (settings_->data_send_[user_index] == 0) ? true : false;
}


std::vector<std::string> CommonResources::getAllVoiceResources(void) {
  return voice_->getResources();
}


std::string CommonResources::getVoiceResource(void) {
  size_t user_index = settings_->user_index_;
  return settings_->voice_resource_[user_index];
}
