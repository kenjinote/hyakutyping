/*!
  \file
  \brief ログ管理クラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "LogManager.h"
#include "LockGuard.h"
#include <string>

using namespace beego;


struct LogManager::pImpl {
  std::string log_fname;
  ErrorLevel handle_level;
  SDL_mutex* mutex;
  FILE* fp;

  pImpl(void)
    : log_fname("errors.txt"), handle_level(Warning),
      mutex(SDL_CreateMutex()), fp(NULL) {
  }

  ~pImpl(void) {
    SDL_DestroyMutex(mutex);
  }
};


LogManager::LogManager(void) : pimpl(new pImpl) {
}


LogManager* LogManager::getObject(int id) {
  static LogManager* obj = new LogManager();
  return obj;
}


LogManager::~LogManager(void) {
}


void LogManager::setFileName(const char* fileName, int id) {
  LockGuard guard(pimpl->mutex);
  pimpl->log_fname = fileName;
}


void LogManager::setLogMask(const ErrorLevel level, int id) {
  LockGuard guard(pimpl->mutex);
  pimpl->handle_level = level;
}


void LogManager::write(const ErrorLevel level, const char* message) {
  if (level > pimpl->handle_level) {
    return;
  }
  LockGuard guard(pimpl->mutex);

  // ログファイルの生成
  if (pimpl->fp == NULL) {
    pimpl->fp = fopen(pimpl->log_fname.c_str(), "w");
    if (pimpl->fp == NULL) {
      perror("LogManager::write");
      return;
    }
  }

  // メッセージの書き込み
  const char* level_message[] = {
    "Unknown", // NoWrite
    "Attack ", // Attack
    "Error  ", // Error
    "Warning", // Warning
    "Notice ", // Notice
  };
  int error_level = (level > Notice) ? 0 : level;
  fprintf(pimpl->fp, "%s: %s", level_message[error_level], message);
  size_t len = strlen(message);
  if (message[len - 1] != '\n') {
    // メッセージの最後が改行でなければ、改行を追加する
    fprintf(pimpl->fp, "%c", '\n');
  }
  fflush(pimpl->fp);
}
