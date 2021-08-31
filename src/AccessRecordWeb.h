#ifndef ACCESS_RECORD_WEB_H
#define ACCESS_RECORD_WEB_H

/*!
  \file
  \brief �f�[�^�̓ǂݏ���(Web)

  \author Satofumi KAMIMURA

  $Id$
*/

#include "AccessRecordInterface.h"


class AccessRecordWeb : public AccessRecordInterface {
  AccessRecordWeb(const AccessRecordWeb& rhs);
  AccessRecordWeb& operator = (const AccessRecordWeb& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  /*!
    \brief �R���X�g���N�^

    \param mode [i] �^�C�s���O���[�h
  */
  AccessRecordWeb(void);
  ~AccessRecordWeb(void);

  bool save(const TypingRecorder::GameSettings& data);
  bool load(std::vector<TypingRecorder::GameSettings>& data,
            const char* user_name,
            time_t from, time_t to, size_t max_num);
};

#endif /*! ACCESS_RECORD_WEB_H */
