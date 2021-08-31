#ifndef TYPING_RECORDER_H
#define TYPING_RECORDER_H

/*!
  \file
  \brief �^�C�s���O���̋L�^

  \author Satofumi KAMIMURA

  $Id$
*/

#include "TypingMode.h"
#include <memory>
#include <string>
#include <vector>


/*!
  \brief �^�C�s���O���̋L�^�N���X
*/
class TypingRecorder {
  TypingRecorder(const TypingRecorder& rhs);
  TypingRecorder& operator = (const TypingRecorder& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  /*!
    \brief �������
  */
  enum {
    Clear = 0x1,                //!< �N���A����
    BackSpace = 0x2,            //!< �o�b�N�X�y�[�X����
    Return = 0x3,               //!< ���s����
    Complete = 0x4,             //!< ���͊���
  };

  /*!
    \brief �L�[���͏��
  */
  typedef struct {
    int ticks;                  //!< ���̓^�C�~���O
    char ch;                    //!< ���̓L�[
  } KeyTiming;

  /*!
    \brief �a�̏��
  */
  typedef struct {
    int no;                      //!< �a�̔ԍ�
    std::vector<KeyTiming> keys; //!< ���̓L�[���
  } WakaTyping;

  /*!
    \brief �L�^�ڍׂ̒�`
  */
  typedef struct {
    int types;                    //!< �^�C�v�� [��]
    int miss_types;               //!< �^�C�v�~�X�̉� [��]
    int delay_msec;               //!< ���͊J�n�x�� [msec]
    int elapse_msec;              //!< �o�ߎ��� [msec]
  } ScoreData;

  /*!
    \brief �Q�[�����
  */
  typedef struct {
    std::string major_version;
    std::string minor_version;
    std::string micro_version;

    TypingMode mode;              //!< �^�C�s���O���[�h
    std::string user_name;        //!< ���[�U��
    time_t time;                  //!< �L�^���ꂽ����
    int rand_seed;                //!< �����_���V�[�h
    ScoreData score;              //!< �X�R�A���
    int waka_num;                 //!< �a�̐�
    std::vector<WakaTyping> waka; //!< �a�̏��
    int kaminoku_speed;           //!< ��̋�̍Đ����x
    int shimonoku_speed;           //!< ���̋�̍Đ����x
  } GameSettings;

  TypingRecorder(void);
  ~TypingRecorder(void);

  /*!
    \brief �Q�[�����̋L�^�J�n

    \param rand_seed [i] �����_���V�[�h
    \param mode [i] �^�C�s���O���[�h
  */
  void recordGame(int rand_seed, TypingMode mode);

  /*!
    \brief �a�̏��̋L�^�J�n

    \param no [i] �a�̔ԍ�
  */
  void recordWaka(size_t no);

  /*!
    \brief �^�C�s���O���̋L�^

    \param ch [i] ���̓L�[
    \param ticks [i] ���̓^�C�s���O
  */
  void recordTyping(char ch, size_t ticks);

  /*!
    \brief �o�^���e�̕]��

    \param user_name [i] ���[�U��
  */
  void evaluate(const char* user_name);

  /*!
    \brief �a�̐��̎擾
  */
  size_t getWakaNum(void);

  /*!
    \brief ���͒x���̎擾
  */
  double getStartDelay(void);

  /*!
    \brief �^�C�s���O�X�s�[�h�̎擾
  */
  double getTypingSpeed(void);

  /*!
    \brief �^�C�v�~�X���̎擾
  */
  size_t getMissTypes(void);

  /*!
    \brief �Q�[���f�[�^��Ԃ�
  */
  GameSettings& getRecordData(void) const;

  /*!
    \brief �Q�[���f�[�^�̃G���R�[�h
  */
  static std::string encodeGameData(const GameSettings& data);

  /*!
    \brief �f�o�b�O�\���p
  */
  void print(void);
};

#endif /* !TYPING_RECORDER_H */
