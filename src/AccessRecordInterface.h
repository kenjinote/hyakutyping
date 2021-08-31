#ifndef ACCESS_RECORD_INTERFACE_H
#define ACCESS_RECORD_INTERFACE_H

/*!
  \file
  \brief �L�^�f�[�^�̑���C���^�[�t�F�[�X

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingRecorder.h"
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <time.h>


/*!
  \brief �^�C�s���O��т̋L�^�C���^�[�t�F�[�X
*/
class AccessRecordInterface {
public:
  virtual ~AccessRecordInterface(void) {}

  /*!
    \brief �L�^

    \param data [i] �L�^�f�[�^

    \retval true ����
    \retval false �G���[
  */
  virtual bool save(const TypingRecorder::GameSettings& data) = 0;

  /*!
    \brief �ǂݍ���

    \param data [o] �ǂݏo���f�[�^
    \param user_name [i] ���[�U��
    \param from [i] �ǂݏo���͈͂̊J�n�ʒu
    \param to [i] �ǂݏo���͈͂̏I���ʒu
    \param max_num [i] �ǂݏo���ő�f�[�^��

    \retval true ����
    \retval false �G���[
  */
  virtual bool load(std::vector<TypingRecorder::GameSettings>& data,
                    const char* user_name,
                    time_t from, time_t to, size_t max_num) = 0;
};

#endif /* !ACCESS_RECORD_INTERFACE_H */
