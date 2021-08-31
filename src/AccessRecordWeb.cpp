/*!
  \file
  \brief �f�[�^�̓ǂݏ���(Web)

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

  // !!! �ȉ����X���b�h������

  // �o�^��
  std::string server = pimpl->common_->getServerName();
  std::string address = pimpl->common_->getServerAddress();

  // ���[�U���A�p�X���[�h�A�f�[�^�̑��M
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
