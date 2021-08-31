/*!
  \file
  \brief �R���\�[���E�R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$

  \todo SDL �łŃt�F�[�h���������삵�Ȃ��̂��C������BOpenGL �łł͓��삷��
*/

#include "ConsolePanelComponent.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "Layer.h"
#include "SdlUtils.h"
#include "PointerRingBuffer.h"
#include <stdarg.h>

using namespace beego;


struct ConsolePanelComponent::pImpl {
  enum {
    NormalScroll,
    ForceScroll,

    NoForcedPosition,
    DefaultFadeBegin = 3000,
    InitialTicks = 0,
  };

  Layer* layer;
  size_t width;
  size_t height;
  TtfResource* ttf;
  size_t font_size;
  size_t last_ticks_;
  SDL_Rect position;
  bool position_changed;
  PointerRingBuffer<DrawSurface*> draw_surfaces;
  Uint32 fore_color;
  Uint32 back_color;

  typedef struct {
    int x;
    int w;
    int h;
    size_t registed_ticks;
    boost::shared_ptr<TextSurface> surface;

    // !!! �������� position_changed ���Ǘ����������A�ĕ`�悪���Ȃ�

  } x_info_t;

  typedef struct {
    int y;
    std::deque<x_info_t> x_info;
    size_t surfaces_size;

    // !!! ���ƁA�s���ł� position_changed ���Ǘ����ׂ�����

  } y_info_t;

  std::deque<y_info_t> surfaces;
  size_t surfaces_size;
  int erase_type;
  int direction;
  size_t next_x;
  size_t next_y;
  bool next_scroll;
  size_t fade_begin;
  size_t fade_period;
  size_t invisible_surfaces;

  // !!! fore, back �� _color �� property ��񂩂�󂯎��ׂ�
  pImpl(size_t w, size_t h, const TextProperty& property,
        EraseType erase_type_, DrawDirection direction_)
    : layer(NULL), width(w), height(h), ttf(property.font),
      font_size(property.size), last_ticks_(0),
      position_changed(false), fore_color(White), back_color(Black),
      surfaces_size(0), erase_type(erase_type_),
      direction(direction_), next_x(0),
      next_y((direction == Down) ? 0 : height - font_size),
      next_scroll(false),
      fade_begin(DefaultFadeBegin), fade_period(DefaultFadePeriod),
      invisible_surfaces(0) {

    set_SdlRect(&position, 0, 0, width, height);

    // �ŏ��̂P�s��ǉ�
    updateToFirstState();
  }

  ~pImpl(void) {
    size_t n = draw_surfaces.size();
    for (size_t i = 0; i < n; ++i) {
      delete draw_surfaces.at(i);
    }
  }

  // �P�s���̃T�[�t�F�X���폜
  void removeLine(void) {

    pImpl::y_info_t& line = surfaces.front();
    size_t remove_size = line.surfaces_size;
    for (size_t i = 0; i < remove_size; ++i) {
        draw_surfaces.rotate();
    }

    surfaces.pop_front();
    surfaces_size -= remove_size;
  }

  // �s�N���A��̍s����������
  void updateToFirstState(void) {
    surfaces.clear();
    surfaces_size = 0;

    y_info_t y_info;
    next_y = (direction == Down) ? 0 : height - font_size;
    y_info.y = next_y;
    y_info.surfaces_size = 0;
    surfaces.push_back(y_info);

    next_scroll = false;
  }

  // �T�[�t�F�X��z�u
  int printf(const char *format, va_list ap) {

    enum { BufferSize = 512 };
    char buffer[BufferSize];

    int n = vsnprintf(buffer, BufferSize, format, ap);
    va_end(ap);

    for (int i = 0; i < n; ++i) {
      Uint16 ch = buffer[i];
      //fprintf(stderr, "ch: %02x, ", ch);
      addCh(ch);
    }
    return n;
  }

  // �`��T�[�t�F�X�ĕ`��̂��߂́A�S����
  void eraseAllSurfaces(void) {
    //fprintf(stderr, "erase all surfaces\n");
    SurfaceList remove_list;
    size_t n = draw_surfaces.size();
    for (size_t i = 0; i < n; ++i) {
      // !!! at() �����K�؂ȃ��\�b�h�ɒu��������
      remove_list.push_back(draw_surfaces.at(i));
    }
    layer->removeSurfaces(remove_list);
  }

  // �P�s�ڂ̍폜�ƈʒu�̍ĕ]�����s��
  void scrollOut(void) {
    if (surfaces.empty()) {
      return;
    }

    //fprintf(stderr, "call scrollOut(): before %d\n", surfaces_size);

    // y �̈ʒu�����炷
    std::deque<y_info_t>::reverse_iterator it = surfaces.rbegin();
    for (std::deque<y_info_t>::reverse_iterator next_it = it + 1;
         next_it != surfaces.rend(); ++next_it, ++it) {
      it->y = next_it->y;
    }

    // �`��T�[�t�F�X�ĕ`��̂��߂́A�S����
    eraseAllSurfaces();

    // �P�s�ڂ̍폜
    // �{���͂����Ŏ�����������Ă͂Ȃ�Ȃ����APointerRingBuffer �Ȃ̂� O.K.
    removeLine();
  }

  void fadeOut(void) {
    if (surfaces.empty()) {
      return;
    }

    // y �̈ʒu���X�V
    int y_offset = (direction == Down) ? +font_size : -font_size;
    for (std::deque<y_info_t>::iterator it = surfaces.begin();
         it != surfaces.end(); ++it) {
      it->y += y_offset;
    }

    // �`��T�[�t�F�X�ĕ`��̂��߂́A�S����
    eraseAllSurfaces();

    // �ł��Â��T�[�t�F�X�ʒu���͈͊O�ɂȂ�ꍇ�ɍ폜
    pImpl::y_info_t& last_line = surfaces.front();
    if (((last_line.y + font_size) > height) ||
        (last_line.y < 0)){
      // �P�s���̃T�[�t�F�X���폜
      removeLine();
    }
  }

  void scroll(int mode = NormalScroll) {

    //size_t adjusted_next_y = next_y;

    if ((erase_type == FadeOut) && (mode == ForceScroll)) {
      // �P�s�ڈȍ~�̍s���X�N���[�������āA�ǉ��̕����𑱂��ĕ`�悳����
      // !!! �����I�ɃX�N���[�����̍Ō�� next_y �̒l��ێ����Ă����K�v������
    }

#if 0
    // ���O�܂ł� ForceDownScroll �̂Ƃ��̏���
    if ((force_next_y == NoForcedPosition) && (mode == NormalScroll)) {
      force_next_y = NoFocedPosition;

      // �ʒu��߂�
    }

    // mode == ForceDownScroll �̂Ƃ��́A���̃J�[�\�������ɂȂ�悤�ɂ���
    if (mode == ForceDownScroll) {

      // Down && ScrollOut �̂Ƃ��́A
      // Down && FadeOut �̂Ƃ��́A
      // !!! ���A���̂�����ׂ̍��������͍���ł悩��

      if (force_next_y == NoFocedPosition) {
        //force_next_y
        // !!!
      }
        // !!! FadeOut �̏ꍇ�A�ꎞ�I�ɁAnext_y ������l�ȊO�ɂ���̂��ȁH
    }

    // Down �̂Ƃ��́A
#endif

    if (erase_type != ScrollOut) {
      // �t�F�[�h�̂Ƃ��́A���s�ɂ���ĕK���S�̂��X�N���[������
      fadeOut();

    } else if (((direction == Down) && ((next_y + font_size) > height)) ||
               ((direction != Down) && (static_cast<int>(next_y) < 0))) {
      // �X�N���[���A�E�g�̏ꍇ�́A�s��ǉ�����Ɣ͈͊O�ɂȂ�ꍇ�ɃX�N���[��
      scrollOut();

    } else {
      // �����ɓ���̂́A���[�h�� ScrollOut �ŃX�N���[�����K�v�Ȃ��Ƃ�
      next_y += font_size * ((direction == Down) ? +1 : -1);
    }

    // X �ʒu�̍X�V
    next_x = 0;

    // Y �ʒu�̍X�V
    y_info_t y_info;
    y_info.y = next_y;
    y_info.surfaces_size = 0;
    surfaces.push_back(y_info);

    position_changed = true;
  }

  void addCh(Uint16 ch) {
    //fprintf(stderr, "first: %d, ", surfaces_size);

    if (ch == ' ') {
      // �󔒂̂Ƃ��́A�����󂯂�B���p��z�肵�A���͔����ɂ��Ă��܂�
      next_x += font_size / 2;

    } else if ((ch == '\n') && (erase_type == ScrollOut)) {
      // ���s����
      scroll();

    } else if ((ch == '\n') && (erase_type == FadeOut)) {
      // ���s�\��
      next_scroll = true;

    } else {
      if ((erase_type == FadeOut) && (next_scroll)) {
        next_scroll = false;
        scroll();
      }

      // �T�[�t�F�X�̍쐬
      x_info_t x_info;
      x_info.registed_ticks = InitialTicks;
      Uint16 text[2] = { 0x0, 0x0 };
      text[0] = ch;

      // !!! �F���w��F�ɒu��������
      TextProperty text_property(ttf, text, font_size,
                                 fore_color, back_color, true);
      boost::shared_ptr<TextSurface> surface(new TextSurface(text_property));
      x_info.w = surface->getWidth();
      x_info.h = surface->getHeight();

      // !!! �����ɂ��Ȃ�A�e�s�̕������̍ő卂�����A���̍s�̍����ɂ��ׂ�
      // !!! font_size ���z�����Ƃ����炢�ł悳�����ȋC�����邪

      if ((next_x + x_info.w) > width) {
        // ���s����
        scroll(ForceScroll);

        next_x = 0;
      }
      std::swap(x_info.surface, surface);
      x_info.x = next_x;
      next_x += x_info.w;

      y_info_t& y_info = surfaces.back();
      y_info.x_info.push_back(x_info);
      ++y_info.surfaces_size;
      ++surfaces_size;

      // �T�[�t�F�X�̓��ꕨ��ǉ�
      if (surfaces_size > draw_surfaces.capacity()) {
        draw_surfaces.push_buffer(new DrawSurface);
      }
      DrawSurface* p = draw_surfaces.get_buffer();
      SDL_Rect dummy_rect;
      set_SdlRect(&dummy_rect, 0, 0, 0, 0);
      updateDrawSurface(*p, x_info.surface, &dummy_rect, last_ticks_,
                        dummy_rect, false);

      position_changed = true;
    }
  }

  bool fadeOutSurface(std::deque<x_info_t>::iterator& x_it, size_t ticks) {

    // �t�F�[�h����
    // !!! �ȉ��A�����Ɠ��삵����ł悢�̂ŁA�֐������Ă���
    if (x_it->registed_ticks == pImpl::InitialTicks) {
      x_it->registed_ticks = ticks;

      return false;
    } else {
      // �t�F�[�h���ʂ̔��f
      size_t first_ticks = x_it->registed_ticks + fade_begin;
      if (ticks < first_ticks) {
        return false;
      }

      size_t percent = 100 * (ticks - first_ticks) / fade_period;
      //fprintf(stderr, "percent: %d, ", percent);
      if (percent > 100) {
        percent = 100;
      }
      size_t adjusted_percent = 100 - percent;
      x_it->surface->setAlpha(adjusted_percent);

      // ���S�ɓ����ɂȂ����̂ŁA�T�[�t�F�X���폜����
      if (adjusted_percent == 0) {
        //fprintf(stderr, "remove: \n");
        // !!!
        // !!! �\���Ώۂɂ��Ȃ���΂悢

        // !!! �T�[�t�F�X�̎������������A�Ƃ����Ӗ��ł́A�폜���ׂ�
        // !!! �폜���Ă݂�
        // �폜�́A�ł��Â��T�[�t�F�X�ɂȂ�͂�
        //removeLatestSurface();
        ++invisible_surfaces;

        return true;
      }
      // !!! ������A���S�ɓ����ɂȂ�����A�T�[�t�F�X���̂��������ׂ�

      return true;
    }
  }

  // �ł��Â��T�[�t�F�X���폜
  void removeLatestSurface(void) {

    y_info_t& first_line = surfaces.front();
    // �s����ɂȂ��΁A�폜���Ă��܂�
    if (first_line.surfaces_size <= 1) {
      //fprintf(stderr, "removeLine\n");
      removeLine();

    } else {
      --first_line.surfaces_size;
      --surfaces_size;
      draw_surfaces.rotate();
      first_line.x_info.pop_front();

      //fprintf(stderr, "(%d, %d, %d, %d), ", surfaces_size, first_line.surfaces_size, surfaces.size(), first_line.x_info.size());
    }
  }

  // ���S�ɓ����ɂȂ����T�[�t�F�X�̓o�^�폜
  void removeInvisibleSurfaces(void) {

    for (size_t i = 0; i < invisible_surfaces; ++i) {
      removeLatestSurface();
    }
    invisible_surfaces = 0;
  }
};


ConsolePanelComponent::ConsolePanelComponent(size_t width, size_t height,
                                             const TextProperty& property,
                                             EraseType erase_type,
                                             DrawDirection direction)
  : pimpl(new pImpl(width, height, property, erase_type, direction)) {
}


ConsolePanelComponent::~ConsolePanelComponent(void) {
}


void ConsolePanelComponent::addSurfaceList(SurfaceList& surfaces,
                                           const SDL_Rect* area,
                                           size_t ticks) {

  pimpl->last_ticks_ = ticks;

  SDL_Rect position;
  size_t index = 0;
  for (std::deque<pImpl::y_info_t>::iterator y_it = pimpl->surfaces.begin();
       y_it != pimpl->surfaces.end(); ++y_it) {
    for (std::deque<pImpl::x_info_t>::iterator x_it = y_it->x_info.begin();
         x_it != y_it->x_info.end(); ++x_it) {

      // position �̍X�V
      set_SdlRect(&position,
                  pimpl->position.x + x_it->x,
                  pimpl->position.y + y_it->y, x_it->w, x_it->h);
      DrawSurface* p = pimpl->draw_surfaces.at(index);
      ++index;

      bool changed = false;
      if (pimpl->erase_type == FadeOut) {
        // �����̃t�F�[�h�A�E�g
        // !!! changed ���֌W�Ȃ��悤�Ȃ�A����
        changed = pimpl->fadeOutSurface(x_it, ticks);
      }
      updateDrawSurface(*p, x_it->surface, area, ticks,
                        position, pimpl->position_changed | changed);

      // �`��T�[�t�F�X��o�^
      surfaces.push_back(p);
    }
  }
  pimpl->position_changed = false;

  // ���S�ɓ����ɂȂ����T�[�t�F�X�̍폜
  pimpl->removeInvisibleSurfaces();
}


void ConsolePanelComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position, position->x, position->y,
              pimpl->position.w, pimpl->position.h);
  pimpl->position_changed = true;
}


void ConsolePanelComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position;
}


size_t ConsolePanelComponent::getWidth(void) {
  return pimpl->width;
}


size_t ConsolePanelComponent::getHeight(void) {
  return pimpl->height;
}


void ConsolePanelComponent::applyInput(const InputEvent& event,
                                       std::vector<SDL_Rect>& applied_rects) {
  // �������Ȃ�
}


void ConsolePanelComponent::registerLayer(Layer* layer) {
  pimpl->layer = layer;
}


int ConsolePanelComponent::printf(const char *fmt, ...) {
  if (! pimpl->layer) {
    // �o�^����ĂȂ��Ƃ��̑���́A��������
    return -1;
  }

  va_list ap;
  va_start(ap, fmt);

  return pimpl->printf(fmt, ap);
}


/*!
  \brief �\�����e�̏���

  \attention �܂�����ɓ��삵�Ȃ�
*/
void ConsolePanelComponent::clear(void) {

  // �T�[�t�F�X���폜
  pimpl->eraseAllSurfaces();
  pimpl->draw_surfaces.clear();

  // ������ԂɍX�V
  pimpl->updateToFirstState();
}


void ConsolePanelComponent::setColor(Uint32 color, Uint32 back_color) {

  pimpl->fore_color = color;
  pimpl->back_color = back_color;
}


void ConsolePanelComponent::setFadeOutMsec(size_t begin, size_t period) {

  pimpl->fade_begin = begin;
  if (period != 0) {
    pimpl->fade_period = period;
  }
}
