/*!
  \file
  \brief HTTP �A�N�Z�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "HttpAccess.h"
#include "NetInit.h"
#include "TcpipCtrl.h"
#include <string>

using namespace beego;


struct HttpAccess::pImpl : private NetInit {
  std::string error_message_;
  TcpipCtrl con_;
  std::string server_;

  pImpl(const char* server, long port)
    : error_message_("no error."), server_(server) {

    if (! con_.connect(server, 80)) {
      // !!! �G���[�� LogManager �o�͂��ׂ�
      //fprintf(stderr, "HttpAccess::pImpl::connect: %s\n", con_.what());

      error_message_ = con_.what();

      // !!! �����́A�ł���Η�O�𓊂���ׂ�
      // !!! �����ȃI�u�W�F�N�g�̐����������ׂ��ł͂Ȃ�
      return;
    }
  }

  bool isLF(char ch) {
    return ((ch == '\n') || (ch == '\r')) ? true : false;
  }
};


HttpAccess::HttpAccess(const char* server, long port)
  : pimpl(new pImpl(server, port)) {
}


HttpAccess::~HttpAccess(void) {
}


const char* HttpAccess::what(void) {
  return pimpl->error_message_.c_str();
}


bool HttpAccess::get(std::vector<char>& page, const char* address) {
  if (! pimpl->con_.isConnected()) {
    return false;
  }

  // !!! CR, LF �𕶎��R�[�h�Ŏw�肷�ׂ�
  // !!! \n �̉��߂��AOS �ɂ���ĈقȂ邽��
  std::string request =
    "GET " + std::string(address) + " HTTP/1.0\r\n"
    "Host: " + pimpl->server_ + "\r\n"
    "\r\n";

  //fprintf(stderr, "request: %s\n", request.c_str());
  pimpl->con_.send(request.c_str(), static_cast<int>(request.size()));

  // !!! �����ƁA�����������ˁH
  // !!! ���[�v���炵�āA�Ђǂ���H
  int lf_count = 0;
  char ch;
  int n;
  bool now_header = true;
  std::string line;
  size_t left_byte = 0;         // �c���M�o�C�g��

  do {
    if (now_header) {
      n = pimpl->con_.recv(&ch, 1, 1000);
    } else {
      // !!! �o�b�t�@�ɓǂݏo���Apage �ɑ΂��Ċi�[���ׂ�
      n = pimpl->con_.recv(&ch, (left_byte > 0 ? 1 : 0), 1000);
      --left_byte;
    }
    if (n > 0) {

      if (! now_header) {
        page.push_back(ch);
      } else {
        // �s���i�[���AContent-Length: �̐��l�����ǂݏo��
        if (isprint(ch)) {
          line.push_back(ch);
        } else {
          if (! line.compare(0, 16, "Content-Length: ")) {
            left_byte = atoi(&line[16]);
          }
          line.clear();
        }
      }

      if (pimpl->isLF(ch)) {
        ++lf_count;
        if (lf_count >= 4) {
          now_header = false;
        }
      } else {
        lf_count = 0;
      }
    }
  } while (n > 0);

  return true;
}
