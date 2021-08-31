/*!
  \file
  \brief データの読み書き(Web)

  \author Satofumi KAMIMURA

  $Id$
*/

#include "AccessRecordWeb.h"
#include "CommonResources.h"
#include "HttpAccess.h"

using namespace beego;


struct AccessRecordWeb::pImpl {
  CommonResources* common_;

  pImpl(void) : common_(CommonResources::getObject()) {
  }
};


AccessRecordWeb::AccessRecordWeb(void) : pimpl(new pImpl) {
}


AccessRecordWeb::~AccessRecordWeb(void) {
}


bool AccessRecordWeb::save(const TypingRecorder::GameSettings& data) {

  // !!! 以下をスレッド化する

  // 登録先
  std::string server = pimpl->common_->getServerName();
  std::string address = pimpl->common_->getServerAddress();

  // ユーザ名、パスワード、データの送信
  address +=
    std::string("registerData.php?name=") + pimpl->common_->getUserName() +
    "&pass=b" + pimpl->common_->getPassword() +
    "&data=" + TypingRecorder::encodeGameData(data);
  //fprintf(stderr, "%s, %s\n", server.c_str(), address.c_str());

  HttpAccess http_access(server.c_str());
  std::vector<char> page_text;
  http_access.get(page_text, address.c_str());

  page_text.push_back('\0');
  //fprintf(stderr, "%s\n", &page_text[0]);

  return false;
}


bool AccessRecordWeb::load(std::vector<TypingRecorder::GameSettings>& data,
                           const char* user_name,
                           time_t from, time_t to, size_t max_num) {
  static_cast<void>(data);
  static_cast<void>(user_name);
  static_cast<void>(from);
  static_cast<void>(to);
  static_cast<void>(max_num);

  // !!!
  return false;
}
