#ifndef CONNECTION_INTERFACE_H
#define CONNECTION_INTERFACE_H

/*!
  \file
  \brief �ʐM�f�o�C�X�̃C���^�[�t�F�[�X��`

  \author Satofumi KAMIMURA

  $Id$
*/


namespace beego {
  /*!
    \brief �ʐM�C���^�[�t�F�[�X
  */
  class ConnectionInterface {
  public:
    virtual ~ConnectionInterface(void) {}

    /*!
      \brief ������Ԃ�Ԃ�

      \return ������Ԃ�����������
    */
    virtual const char* what(void) = 0;

    /*!
      \brief �f�o�C�X�ւ̐ڑ�

      \param device [i] �ڑ��f�o�C�X��
      \param baudrate [i] �ڑ��{�[���[�g

      \retval true ����
      \retval false ���s
    */
    virtual bool connect(const char* device, long baudrate) = 0;

    /*!
      \brief �ؒf
    */
    virtual void disconnect(void) = 0;

    /*!
      \brief �ڑ���Ԃ�Ԃ�

      \retval true �ڑ���
      \retval false �ؒf��
    */
    virtual bool isConnected(void) = 0;

    /*!
      \brief �{�[���[�g�̕ύX

      \param baudrate [i] �{�[���[�g

      \retval true ����ɕύX
      \retval false �ύX�Ɏ��s
    */
    virtual bool changeBaudrate(long baudrate) = 0;

    /*!
      \brief ���M

      \param data [i] ���M�f�[�^
      \param size [i] ���M�o�C�g��

      \return ���M�����o�C�g��
    */
    virtual int send(const char* data, int size) = 0;

    /*!
      \brief ��M

      \param data [o] ��M�p�o�b�t�@
      \param size [i] ��M�o�b�t�@�̍ő�T�C�Y
      \param timeout [i] �^�C���A�E�g���� [msec]
    */
    virtual int recv(char* data, int size, int timeout) = 0;

    /*!
      \brief ��M�ς݃f�[�^�̃o�C�g����Ԃ�

      \param timeout [i] �^�C���A�E�g���� [msec]

      \return ��M�ς݃f�[�^�̃o�C�g��
    */
    virtual int size(int timeout = 0) = 0;

    /*!
      \brief ����M�f�[�^�̃N���A

      ���M�o�b�t�@�A��M�ς݃f�[�^���N���A����
    */
    virtual void clear(void) = 0;

    /*!
      \brief ��M�f�[�^�̔j��

      ��M�f�[�^��ǂݎ̂Ă�

      \param total_timeout [i] ��M�g�[�^���̃^�C���A�E�g���� [msec]
    */
    virtual void skip(int total_timeout) = 0;

    virtual void flush(void) = 0;
  };
};

#endif /* !CONNECTION_INTERFACE_H */
