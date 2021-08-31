#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

/*!
  \file
  \brief ���O�Ǘ��N���X

  \author Satofumi KAMIMURA

  $Id$

  \todo ID �ԍ��ŏ������݃t�@�C�����w��ł���悤�ɕύX����
*/

#include <memory>


namespace beego {
  /*!
    \brief ���O�Ǘ��N���X
  */
  class LogManager {
    LogManager(void);
    LogManager(const LogManager& rhs);
    LogManager& operator = (const LogManager& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    /*!
      \brief �G���[���x��
    */
    typedef enum {
      NoWrite = 0,                //!< �S���b�Z�[�W�̖���
      Attack,                        //!< �N���b�N
      Error,                        //!< �G���[
      Warning,                        //!< �x��
      Notice,                        //!< ����
      All,                        //!< �S���b�Z�[�W�̋L�^
    } ErrorLevel;

    ~LogManager(void);
    static LogManager* getObject(int id = 0);

    /*!
      \brief ���O�t�@�C�������w�肷��

      ���̊֐��́A�ŏ��� write() ���Ă΂��O�ɌĂяo����Ȃ���΂Ȃ�Ȃ��B

      \param fileName [i] ���O�t�@�C����
    */
    void setFileName(const char* fileName, int id = 0);


    /*!
      \brief �t�@�C���ɋL�^���Ȃ��G���[���x�����w�肷��

      �w�肳�ꂽ�G���[���x�������Ⴂ�L�^���t�@�C���ɏ������܂Ȃ��B

      \param mask [i] �������݂��s���G���[���x��

      \code
      LogManager* log = LogManager::getObject();
      log->setFileName("error_log.txt");

      // Warning, Notice �̓t�@�C���ɏ������܂�Ȃ��Ȃ�
      log->setLogMask(Error);
      ...

      log->write(Error, "error!");        // �������܂��
      log->write(Warning, "warning!");         // �������܂�Ȃ�
      \endcode
    */
    void setLogMask(const ErrorLevel level, int id = 0);


    /*!
      \brief ���O�Ƀ��b�Z�[�W����������

      ���̃��\�b�h���Ƀ��b�Z�[�W�͉��s����ċL�^�����B

      \param level [i] ���O���x��
      \param message [i] ���b�Z�[�W
    */
    void write(const ErrorLevel level, const char* message);
  };
};

#endif /* !LOG_MANAGER_H */
