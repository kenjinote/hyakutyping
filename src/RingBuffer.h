#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/*!
  \file
  \brief �����O�o�b�t�@

  \author Satofumi KAMIMURA

  $Id$
*/

#include <vector>


namespace beego {
  /*!
    \brief �����O�o�b�t�@
  */
  template<class T> class RingBuffer {
    RingBuffer(const RingBuffer& rhs);
    RingBuffer& operator = (const RingBuffer& rhs);

    std::vector<T> ring_buffer;
    size_t buffer_size;
    size_t first;
    size_t last;

    size_t free_size(void) {
      return buffer_size -1 - size();
    }

  public:
    explicit RingBuffer(size_t defaultSize = 0)
      : buffer_size(0), first(0), last(0) {
      if (defaultSize > 0) {
        size_t buffer_size = 1;
        while (buffer_size < defaultSize) {
          buffer_size *= 2;
        }
        ring_buffer.resize(buffer_size);
      }
    }


    ~RingBuffer(void) {
    }

    size_t size(void) {
      return (last >= first) ? last - first : buffer_size - (first - last);
    }

    bool empty(void) {
      return (first == last) ? true : false;
    }

    void put(const T* data, size_t size) {

      long need_size = size - free_size();
      if (need_size > 0) {
        // �o�b�t�@�ɓ��肫��Ȃ��ꍇ�A�g��
        size_t extend_size = (buffer_size == 0) ? 1 : buffer_size;
        while (need_size > 0) {
          need_size -= extend_size;
          extend_size *= 2;
        }
        ring_buffer.resize(extend_size);

        // �g���T�C�Y�ɍ��킹�ăf�[�^���ړ�
        if (first > last) {
          // 0 ���� last �܂ł̃f�[�^���Abuffer_size �ȍ~�Ɉړ�������
          memmove(&ring_buffer[buffer_size],
                  &ring_buffer[0], last * sizeof(T));
          last += buffer_size;
        }
        buffer_size = extend_size;
      }
      // �f�[�^�z�u
      if (first <= last) {
        // last ���� buffer_size �I�[�܂łɔz�u
        size_t to_end = buffer_size - last;
        size_t move_size = (to_end > size) ? size : to_end;
        memmove(&ring_buffer[last], data, move_size * sizeof(T));
        last += move_size;
        last &= (buffer_size -1);

        long left_size = size - move_size;
        if (left_size > 0) {
          // 0 ���� first �̑O�܂ł�z�u
          memmove(&ring_buffer[0], &data[move_size], left_size * sizeof(T));
          last = left_size;
        }
      } else {
        // last ���� first �̑O�܂Ŕz�u
        memmove(&ring_buffer[last], data, size * sizeof(T));
        last += size;
      }
    }

    size_t get(T* data, size_t size) {
      // �f�[�^�擾
      size_t return_size = (size <= this->size()) ? size : this->size();

      if (first <= last) {
        memmove(data, &ring_buffer[first], return_size * sizeof(T));
        first += return_size;
      } else {
        // first ���� buffer_size �I�[�܂ł�z�u
        size_t to_end = buffer_size - first;
        size_t move_size = (to_end > return_size) ? return_size : to_end;
        memmove(data, &ring_buffer[first], move_size * sizeof(T));
        first += move_size;
        first &= (buffer_size -1);

        long left_size = return_size - move_size;
        if (left_size > 0) {
          // 0 ���� last �̑O�܂ł�z�u
          memmove(&data[move_size], &ring_buffer[0], left_size * sizeof(T));
          first = left_size;
        }
      }
      return return_size;
    }

    void clear(void) {
      first = 0;
      last = 0;
    }
  };
};

#endif /* !RING_BUFFER_H */
