#ifndef THREAD_CREATOR_SDL_H
#define THREAD_CREATOR_SDL_H

/*!
  \file
  \brief SDL_Thread �ɂ��X���b�h�n���h��

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


namespace beego {
  /*!
    \brief �X���b�h�̐����N���X
  */
  class ThreadCreator {
    ThreadCreator(void);
    ThreadCreator(const ThreadCreator& rhs);
    ThreadCreator& operator = (const ThreadCreator& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    explicit ThreadCreator(int (*fn)(void *), void* args);
    ~ThreadCreator(void);

    /*!
      \brief �X���b�h�̋N��
    */
    void run(void);

    /*!
      \brief �����񐔂��w�肵�ăX���b�h���N��

      \param times [i] ������
    */
    void run(size_t times);

    /*!
      \brief �X���b�h���~
    */
    void stop(void);

    /*!
      \brief �X���b�h�̏I����҂�

      \param retval [o] �X���b�h�̖߂�l���i�[�����
    */
    void wait(int* retval = NULL);

    /*!
      \brief �X���b�h�����쒆����Ԃ�

      \retval true ���쒆
      \retval false ��~��
    */
    bool isRunning(void);
  };
};

#endif /* !THREAD_CREATOR_SDL_H */
