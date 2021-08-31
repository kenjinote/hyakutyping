#ifndef ACCESS_RECORD_PC_H
#define ACCESS_RECORD_PC_H

/*!
  \file
  \brief �f�[�^�̓ǂݏ���(���[�J��)

  \author Satofumi KAMIMURA

  $Id$
*/

#include "AccessRecordInterface.h"


/*!
  \brief PC ���ɐ�т�ۑ�
*/
class AccessRecordPC : public AccessRecordInterface {
  AccessRecordPC(void);
  AccessRecordPC(const AccessRecordPC& rhs);
  AccessRecordPC& operator = (const AccessRecordPC& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

 public:
  /*!
    \brief �R���X�g���N�^

    \param mode [i] �^�C�s���O���[�h
  */
  AccessRecordPC(const TypingMode mode);
  ~AccessRecordPC(void);

  bool save(const TypingRecorder::GameSettings& data);
  bool load(std::vector<TypingRecorder::GameSettings>& data,
            const char* user_name,
            time_t from, time_t to, size_t max_num);
};

#endif /* !ACCESS_RECORD_PC_H */
