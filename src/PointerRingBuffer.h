#ifndef POINTER_RING_BUFFER_H
#define POINTER_RING_BUFFER_H

/*!
  \file
  \brief �|�C���^�p�̃����O�o�b�t�@

  �z��̈���|�C���^�ŊǗ����A�������̈�̎擾�Ɖ���̉񐔂����炷�̂ɗp����

  \author Satofumi KAMIMURA

  $Id$
*/

#include <deque>


namespace {
  /*!
    \brief �|�C���^�p�̃����O�o�b�t�@
  */
  template<class T> class PointerRingBuffer {
    PointerRingBuffer(const PointerRingBuffer& rhs);
    PointerRingBuffer& operator = (const PointerRingBuffer& rhs);

    std::deque<T> ring_buffer;
    size_t filled;

  public:
    PointerRingBuffer(void) : filled(0) {
    }

    ~PointerRingBuffer(void) {
    }

    /*!
      \brief �Ǘ����Ă���|�C���^����Ԃ�

      \return �Ǘ����Ă���|�C���^��
    */
    size_t capacity(void) {
      return ring_buffer.size();
    }

    /*!
      \brief ���g�̗L���ȃ|�C���^����Ԃ�

      \return ���g�̗L���ȃ|�C���^��
    */
    size_t size(void) {
      return filled;
    }

    /*!
      \brief �擪�|�C���^��Ԃ�

      \return �擪�|�C���^
    */
    T front(void) {
      return ring_buffer.front();
    }

    /*!
      \brief �|�C���^�̒ǉ�

      \param buffer [i] �|�C���^�̒ǉ�
    */
    void push_buffer(T buffer) {
      ring_buffer.push_back(buffer);
    }

    /*!
      \brief �|�C���^�̎擾

      �擾�����|�C���^�ɂ́A�L���ȃf�[�^�������邱�Ƃ����߂��Ă���

      \return �|�C���^
    */
    T get_buffer(void) {
      T ret = ring_buffer[filled++];
      return ret;
    }

    /*!
      \brief ���g�̗L���ȃ|�C���^���P���炷

      get_buffer() ����̎��������s��
    */
    void revert_buffer(void) {
      --filled;
    }

    /*!
      \brief �擪�|�C���^�𖖔��Ɉړ�������
    */
    void rotate(void) {
      T first = front();
      ring_buffer.pop_front();
      --filled;

      ring_buffer.push_back(first);
    }

    /*!
      \brief ���g�̗L���ȃ|�C���^�����[���ɂ���

      \attention �|�C���^�̈�̉���͍s��Ȃ�
    */
    void clear(void) {
      filled = 0;
    }

    T at(size_t index) {
      return ring_buffer.at(index);
    }
  };
};

#endif /* !POINTER_RING_BUFFER_H */
