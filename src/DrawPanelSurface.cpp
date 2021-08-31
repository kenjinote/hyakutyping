/*!
  \file
  \brief �`��p�l���T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  \todo �͈͊O�ɕ`�悵�Ȃ��悤�ɂ���
  \todo �h�b�g�P�ʂł� update_rects() �ɓo�^���������������ǂ�����]������
  \todo setNoBackground() ���Ăяo���ƁA�폜�̈悪���������Ȃ�̂��ǂ��ɂ�����
*/

#include "DrawPanelSurface.h"
#include "DrawPrimitive.h"
#include "GuiColors.h"
#include "TextSurface.h"
#include "SdlUtils.h"
#include "LogManager.h"
#include <boost/shared_ptr.hpp>
#include <string>

using namespace beego;


/*!
  \brief DrawPanelSurface �̓����N���X
*/
struct DrawPanelSurface::pImpl {
  size_t width;
  size_t height;

  // �����N���X����A�N�Z�X���邽�߁Astatic �����o�ɂ��Ă���
  //static SDL_Surface* scr;

  class DrawObjectInterface {
  public:
    virtual ~DrawObjectInterface(void) {}
    virtual void draw(const SDL_Rect* pos, const SDL_Rect* area) = 0;
  };

  /*!
    \brief �_�̕`��
  */
  class DrawPointObject : public DrawObjectInterface {
    int px;
    int py;
    Uint32 color;

  public:
    DrawPointObject(int x, int y, Uint32 c)
      : px(x), py(y), color(draw_getDrawColor(c)) {
    }

    ~DrawPointObject(void) {
    }

    void draw(const SDL_Rect* pos, const SDL_Rect* area) {
      beego::draw_point(pos->x + px, pos->y + py, color);
      //drawPoint(pos->x + px, pos->y + py, color);
    }
  };

  /*!
    \brief ���̕`��
  */
  class DrawLineObject : public DrawObjectInterface {
    Grid<int> p1_;
    Grid<int> p2_;
    Uint32 color_;

  public:
    DrawLineObject(const Grid<int>& p1, const Grid<int>& p2, Uint32 c)
      : p1_(p1), p2_(p2), color_(draw_getDrawColor(c)) {
    }

    void draw(const SDL_Rect* pos, const SDL_Rect* area) {

      // pos �ʒu�𔽉f������
      int x1 = p1_.x + pos->x;
      int y1 = p1_.y + pos->y;
      int x2 = p2_.x + pos->x;
      int y2 = p2_.y + pos->y;

      draw_line(x1, y1, x2, y2, color_);
    }
  };

  /*!
    \brief ��`�̕`��
  */
  class DrawRectObject : public DrawObjectInterface {
    Grid<int> position_;
    size_t w_;
    size_t h_;
    Uint32 color_;

  public:
    DrawRectObject(const Grid<int>& position,
                   size_t w, size_t h, Uint32 color)
      : position_(position), w_(w), h_(h), color_(draw_getDrawColor(color)) {
    }

    void draw(const SDL_Rect* pos, const SDL_Rect* area) {

      // pos �ʒu�𔽉f������
      int x = position_.x + pos->x;
      int y = position_.y + pos->y;

      draw_rect(x, y, w_, h_, color_);
    }
  };

  /*!
    \brief ������̕`��
  */
  class DrawTextObject : public DrawObjectInterface {
    Grid<int> position_;
    boost::shared_ptr<TextSurface> text_;

  public:
    DrawTextObject(const Grid<int>& position,
                   const TextProperty& text, int mode)
      : position_(position), text_(new TextSurface(text)) {

      if (mode == Center) {
        position_.x -= text_->getWidth() / 2;
      }
    }

    void draw(const SDL_Rect* pos, const SDL_Rect* area) {

      // pos �ʒu�𔽉f������
      int x = position_.x + pos->x;
      int y = position_.y + pos->y;

      draw_text(x, y, text_);
    }
  };

  typedef std::vector<DrawObjectInterface*> DrawObject;
  DrawObject object_list;
  bool is_changed;
  bool have_background_color;
  Uint32 background_color;

  pImpl(size_t w, size_t h)
    : width(w), height(h), is_changed(false), have_background_color(true) {

    SDL_Surface* scr = SDL_GetVideoSurface();
    beego::draw_registerScrResource(scr);
    if (scr == NULL) {
      LogManager* log = LogManager::getObject();
      std::string message =
        "DrawPanelSurface::pImpl(): Screen surface must be created.";
      log->write(LogManager::Warning, message.c_str());
      return;
    }
    background_color = getSdlColor(scr, Black);
  }

  void clear(void) {
    if (! object_list.empty()) {
      is_changed = false;
    }
    for (DrawObject::iterator it = object_list.begin();
         it != object_list.end(); ++it) {
      delete *it;
    }
    object_list.clear();
  }

  void draw(const SDL_Rect* pos, const SDL_Rect* area) {
    for (DrawObject::iterator it = object_list.begin();
         it != object_list.end(); ++it) {
      (*it)->draw(pos, area);
    }
  }
};


DrawPanelSurface::DrawPanelSurface(size_t width, size_t height)
  : pimpl(new pImpl(width, height)) {
}


DrawPanelSurface::~DrawPanelSurface(void) {
}


void DrawPanelSurface::draw(std::vector<SDL_Rect>& update_rects,
                            const SDL_Rect* pos, const SDL_Rect* area) {
  // ��ʂ̃N���A
  // !!! �ʒu�� 0,0 �ȊO�̏ꏊ���ƁA�����Ɠ��삵�Ȃ��񂶂�Ȃ����H
  // !!! �v����
  // !!! addSurfaceList() �ŁApos �ɂ͐�Έʒu��^����ׂ�

  SDL_Rect clear_area;
  set_SdlRect(&clear_area,
              //pos->x + area->x, pos->y + area->y, area->w, area->h);
              pos->x, pos->y, area->w, area->h);
  //if (pimpl->have_background_color) {
  beego::draw_rect(&clear_area, pimpl->background_color);
  //}

  // �`��I�u�W�F�N�g�����ɕ`�悷��
  //fprintf(stderr, "draw[");
  pimpl->draw(pos, area);
  //fprintf(stderr, "], ");

  update_rects.push_back(clear_area);
}


size_t DrawPanelSurface::getWidth(void) {
  return pimpl->width;
}


size_t DrawPanelSurface::getHeight(void) {
  return pimpl->height;
}


void DrawPanelSurface::forceSetChanged(void) {
  pimpl->is_changed = true;
}


bool DrawPanelSurface::isChanged(size_t ticks) {
  bool ret = pimpl->is_changed;
  pimpl->is_changed = false;
  return ret;
}


bool DrawPanelSurface::isTransparent(void) {
  return true;
}


void DrawPanelSurface::clear(void) {
  pimpl->clear();
}


#if 0
void DrawPanelSurface::setFontResource(TtfResource* font) {
  // !!!
}
#endif


void DrawPanelSurface::setBackgroundColor(Uint32 color) {

  pimpl->background_color = draw_getDrawColor(color);
  pimpl->have_background_color = true;
}


void DrawPanelSurface::setNoBackground(void) {
  pimpl->have_background_color = false;
}


void DrawPanelSurface::drawPoint(const Grid<int>& point, Uint32 color) {
  pImpl::DrawObjectInterface* p =
    new pImpl::DrawPointObject(point.x, point.y, color);
  pimpl->object_list.push_back(p);

  pimpl->is_changed = true;
}


void DrawPanelSurface::drawPoints(const std::vector<Grid<int> >& points,
                                  Uint32 color) {
  for (std::vector<Grid<int> >::const_iterator it = points.begin();
       it != points.end(); ++it) {
    drawPoint(*it, color);
  }

  pimpl->is_changed = true;
}


void DrawPanelSurface::drawLine(const Grid<int>& p1, const Grid<int>& p2,
                                Uint32 color) {

  // !!! �`�悳����
  // !!! �͂ݏo���̔���ƃN���b�s���O�̏����́A�����Ɏ������Ă��܂��܂��傤

  pImpl::DrawObjectInterface* p = new pImpl::DrawLineObject(p1, p2, color);
  pimpl->object_list.push_back(p);
  pimpl->is_changed = true;
}


void DrawPanelSurface::drawLines(const std::vector<Grid<int> >& points,
                                 Uint32 color) {
  if (points.empty()) {
    return;
  }

  const Grid<int>* first = &points[0];
  for (std::vector<Grid<int> >::const_iterator it = points.begin();
       it != points.end(); ++it) {
    const Grid<int>& last = *it;
    drawLine(*first, last, color);
    first = &last;
  }

  pimpl->is_changed = true;
}


void DrawPanelSurface::drawRect(const Grid<int>& position,
                                size_t w, size_t h, Uint32 color) {

  pImpl::DrawObjectInterface* p =
    new pImpl::DrawRectObject(position, w, h, color);
  pimpl->object_list.push_back(p);
  pimpl->is_changed = true;
}


void DrawPanelSurface::drawText(const Grid<int>& position,
                                const TextProperty& text, int mode) {

  pImpl::DrawObjectInterface* p =
    new pImpl::DrawTextObject(position, text, mode);
  pimpl->object_list.push_back(p);

  pimpl->is_changed = true;
}
