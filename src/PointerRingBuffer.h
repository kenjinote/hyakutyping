#ifndef POINTER_RING_BUFFER_H
#define POINTER_RING_BUFFER_H

/*!
  \file
  \brief ポインタ用のリングバッファ

  配列領域をポインタで管理し、メモリ領域の取得と解放の回数を減らすのに用いる

  \author Satofumi KAMIMURA

  $Id$
*/

#include <deque>


namespace {
  /*!
    \brief ポインタ用のリングバッファ
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
      \brief 管理しているポインタ数を返す

      \return 管理しているポインタ数
    */
    size_t capacity(void) {
      return ring_buffer.size();
    }

    /*!
      \brief 中身の有効なポインタ数を返す

      \return 中身の有効なポインタ数
    */
    size_t size(void) {
      return filled;
    }

    /*!
      \brief 先頭ポインタを返す

      \return 先頭ポインタ
    */
    T front(void) {
      return ring_buffer.front();
    }

    /*!
      \brief ポインタの追加

      \param buffer [i] ポインタの追加
    */
    void push_buffer(T buffer) {
      ring_buffer.push_back(buffer);
    }

    /*!
      \brief ポインタの取得

      取得したポインタには、有効なデータを代入することが求められている

      \return ポインタ
    */
    T get_buffer(void) {
      T ret = ring_buffer[filled++];
      return ret;
    }

    /*!
      \brief 中身の有効なポインタを１つ減らす

      get_buffer() 操作の取り消しを行う
    */
    void revert_buffer(void) {
      --filled;
    }

    /*!
      \brief 先頭ポインタを末尾に移動させる
    */
    void rotate(void) {
      T first = front();
      ring_buffer.pop_front();
      --filled;

      ring_buffer.push_back(first);
    }

    /*!
      \brief 中身の有効なポインタ数をゼロにする

      \attention ポインタ領域の解放は行わない
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
