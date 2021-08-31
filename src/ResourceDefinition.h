#ifndef RESOURCE_DEFINITION_H
#define RESOURCE_DEFINITION_H

/*!
  \file
  \brief リソース定義

  \author Satofumi KAMIMURA

  $Id$
*/

#include "DetectOS.h"


// 設定ファイル
const char*const ConfigFile = "hyakutypingrc.txt"; //!< 設定ファイル


/*!
  \brief デフォルトのサーバアドレス
*/
#ifndef DEFAULT_SERVER_ADDRESS
const char*const DefaultServerAddress = "hyakutyping.sourceforge.jp/";
#else
const char*const DefaultServerAddress = DEFAULT_SERVER_ADDRESS;
#endif


// 音楽まわり
enum {
  BackMusic_1 = 1,              //!< 背景曲１
  BackMusic_2,                  //!< 背景曲２
  SoundEffect_1,                //!< 効果音１
  SoundEffect_2,                //!< 効果音２
  Decide,                       //!< 決定音
  Type,                         //!< タイプ音
  Miss,                         //!< ミス音

  FloatingEffectId,             //!< 任意の効果音
};


// 保存ディレクトリ
#if !defined(PACKAGE_DATA_DIR) || defined(WINDOWS_OS)
const char*const FontPath = "resource/font.ttf"; //!< フォントパス
const char*const SaveDir = "savedata/"; //!< 保存ディレクトリ
const char*const VoiceDir = "voicedata/"; //!< 詠み上げ用
const char*const MusicDirectory = "resource/"; //!< 音楽資源の場所
const char*const Music1_FilePath = "resource/music1.mp3"; //!< 背景曲１
const char*const Music2_FilePath = "resource/music2.mp3"; //!< 背景曲２
const char*const Effect1_FilePath = "resource/effect1.wav"; //!< 開始音
const char*const Effect2_FilePath = "resource/effect2.wav"; //!< 終了音
const char*const Decide_FilePath = "resource/decide.wav";   //!< 決定音
const char*const Miss_FilePath = "resource/miss.wav";       //!< ミス音

#else
const char*const FontPath = PACKAGE_DATA_DIR "/font.ttf"; //!< フォントパス
const char*const SaveDir = "~/.hyakutyping/"; //!< 保存ディレクトリ
const char*const VoiceDir = PACKAGE_DATA_DIR "/";      //!< 詠み上げ用
const char*const MusicDirectory = PACKAGE_DATA_DIR "/";//!< 音楽資源の場所
const char*const Music1_FilePath = PACKAGE_DATA_DIR "/music1.mp3"; //!< 背景曲１
const char*const Music2_FilePath = PACKAGE_DATA_DIR "/music2.mp3";//!< 背景曲２
const char*const Effect1_FilePath = PACKAGE_DATA_DIR "/effect1.wav"; //!< 開始音
const char*const Effect2_FilePath = PACKAGE_DATA_DIR "/effect2.wav"; //!< 終了音
const char*const Decide_FilePath = PACKAGE_DATA_DIR "/decide.wav"; //!< 決定音
const char*const Miss_FilePath = PACKAGE_DATA_DIR "/miss.wav";     //!< ミス音
#endif

#endif /* !RESOURCE_DEFINITION_H */
