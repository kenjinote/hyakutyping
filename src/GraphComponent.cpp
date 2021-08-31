/*!
  \file
  \brief �O���t�`��

  \author Satofumi KAMIMURA

  $Id$

  \todo drawTitle() �� registerTitle() �Ƃ��ď����ł���悤�ɂ���
  \todo �o�^�_���P�̂Ƃ����A�O���t�𐶐��ł���悤�ɂ���
*/

#include "GraphComponent.h"
#include "Component.h"
#include "GuiColors.h"
#include "ColorSurface.h"
#include "TextSurface.h"
#include "DrawPanelSurface.h"
#include "TextProperty.h"
#include "SdlUtils.h"
#include "MathUtils.h"

using namespace beego;


struct GraphComponent::pImpl {

  enum {
    TextOffset = 4,
    NumberSize = 14,

    LegendLineLength = 24,
  };

  typedef struct {
    std::vector<Grid<double> > points; //!< ��
    Uint32 color;                      //!< �F
  } line_t;

  size_t width_;
  size_t height_;
  SDL_Rect position_;
  bool position_changed_;

  DrawSurface draw_surfaces_;
  boost::shared_ptr<DrawPanelSurface> graph_surface_;
  bool have_background_;
  Uint32 background_color_;
  Uint32 fore_color_;
  size_t data_max_;
  bool created_;

  double x_magnify_;
  double y_magnify_;
  int x_offset_;
  int y_offset_;
  Grid<int> graph_size_;
  std::vector<line_t*> draw_lines_;
  double x_min_;
  double x_max_;
  double y_min_;
  double y_max_;

  std::vector<TextProperty*> line_titles_;
  std::vector<Uint32> line_colors_;

  pImpl(void)
    : width_(0), height_(0), position_changed_(false),
      have_background_(false), background_color_(Black), fore_color_(White),
      data_max_(0), created_(false), x_magnify_(0.0), y_magnify_(0.0),
      x_offset_(0), y_offset_(0),
      x_min_(0.0), x_max_(0.0), y_min_(0.0), y_max_(0.0) {

    set_SdlRect(&position_, 0, 0, width_, height_);
  }

  void addSurfaceList(SurfaceList& surfaces,
                      const SDL_Rect* area, size_t ticks) {
    if (created_) {
      // �O���t
      SDL_Rect draw_area;
      set_SdlRect(&draw_area, position_.x, position_.y,
                  width_, height_);
      updateDrawSurface(draw_surfaces_,
                        graph_surface_, area, ticks,
                        draw_area, position_changed_);
      surfaces.push_back(&draw_surfaces_);
    }
  }

  // ���̐��l�����̍쐬
  void createAxes(TtfResource* font) {

    // �⏕���̐F���쐬
    // !!! ���A�֐������܂��傤
    // !!! �}�X�N�́A�G���f�B�A���ˑ��Ȃ̂ŁA�����I�ɏC�����ׂ��A�������H
    enum {
      RedMask = 0xff000000,
      GreenMask = 0x00ff0000,
      BlueMask = 0x0000ff00,
    };
    Uint32 r =
      static_cast<Uint32>(((fore_color_ & RedMask) +
                           (background_color_ & RedMask) * 4.0) / 5);
    Uint32 g =
      static_cast<Uint32>(((fore_color_ & GreenMask) +
                           (background_color_ & GreenMask) * 4.0) / 5);
    Uint32 b =
      static_cast<Uint32>(((fore_color_ & BlueMask) +
                           (background_color_ & BlueMask) * 4.0) / 5);
    Uint32 line_color = (r & RedMask) | (g & GreenMask) | (b & BlueMask) | 0xff;

    // !!! �Ƃ肠�����A���l�̑����� 2 �Œ��
    // !!! �J�n���A0 �����
    int min = static_cast<int>(ceil(y_min_ - 0.1));
    int max = static_cast<int>(ceil(y_max_ + 0.1));

    for (int i = min; i < max; ++i) {
      char number[] = "012345678901234";
      sprintf(number, "%d", i);
      TextProperty num_property(font, number, NumberSize,
                                fore_color_, background_color_, true);
      Grid<int> position = convertGraphPoint(Grid<double>(x_max_ * 1.07, i));
      position.y -= NumberSize * 3 / 5;
      graph_surface_->drawText(position, num_property, Center);

      Grid<int> p0 = convertGraphPoint(Grid<double>(0, i));
      Grid<int> p1 = convertGraphPoint(Grid<double>(x_max_ * 1.05, i));
      graph_surface_->drawLine(p0, p1, line_color);
    }

    // X ���̕⏕����`��
    // !!! Y ���`��Ƃ̏d�����Ȃ�Ƃ����ׂ�
    min = static_cast<int>(ceil(x_min_ - 0.1));
    max = static_cast<int>(ceil(x_max_ + 0.1));
    for (int i = min; i < max; ++i) {
      Grid<int> p0 = convertGraphPoint(Grid<double>(i, 0));
      Grid<int> p1 = convertGraphPoint(Grid<double>(i, y_max_ * 1.05));
      graph_surface_->drawLine(p0, p1, line_color);
    }

    // !!!
  }

  Grid<int> convertGraphPoint(Grid<double> original) {

    int x = static_cast<int>(original.x * x_magnify_);
    int y = graph_size_.y - static_cast<int>(original.y * y_magnify_) -1;

    return Grid<int>(x + x_offset_, y + y_offset_);
  }

  void create(size_t width, size_t height, TtfResource* font) {

    graph_size_.x = width;
    graph_size_.y = height;
    width_ = width;
    height_ = height;

    // �f�[�^���P�̎��́A�\���͈͂�ύX���āA�̍ق𐮂���
    // !!! ����

    x_offset_ = 0;
    y_offset_ = 0;
    if (font) {
      // �����������o�^���ꂽ�Ƃ��̂݁A���̐��l�Ǝ��̖��O�𐶐�����

      // ���̐��l�A����ю��̖��O�����̕��̌v�Z
      graph_size_.x -= TextOffset * 4 + NumberSize * 2;
      graph_size_.y -= TextOffset * 5;

      x_offset_ = TextOffset * 2;
      y_offset_ = TextOffset * 2;
    }

    // �O���t�̍쐬
    boost::shared_ptr<DrawPanelSurface>
      new_graph_surface(new DrawPanelSurface(graph_size_.x, graph_size_.y));
    std::swap(graph_surface_, new_graph_surface);

    // �O���t�w�i�̓h��Ԃ�
    if (have_background_) {
      graph_surface_->setBackgroundColor(background_color_);
    }

    x_magnify_ = graph_size_.x / ((x_max_ - x_min_) * 1.05);
    y_magnify_ = graph_size_.y / ((y_max_ - y_min_) * 1.05);

    if (font) {
      if (data_max_ > 1) {
        // �f�[�^������΁A�\������

        // ���̐��l�̐���
        createAxes(font);

        // ���̕`��
        std::vector<Grid<int> > points;
        points.push_back(convertGraphPoint(Grid<double>(0.0, y_max_ * 1.05)));
        points.push_back(convertGraphPoint(Grid<double>(0.0, 0.0)));
        points.push_back(convertGraphPoint(Grid<double>(x_max_ * 1.05, 0.0)));
        graph_surface_->drawLines(points, fore_color_);
      }
    }

    // �O���t���̕`��
    if (data_max_ == 0) {
      // �f�[�^���Ȃ��ꍇ�A���̎|�����b�Z�[�W�ŏo�͂���
      Grid<int> position(position_.x + width_/2, position_.y + height/2);
      TextProperty no_data(font, "No Data.", 16,
                           fore_color_, background_color_, true);
      graph_surface_->drawText(position, no_data, Center);

    } else if (data_max_ == 1) {
      // �f�[�^���P�̏ꍇ���A�\���ł��Ȃ��|�̃��b�Z�[�W���o�͂���
      Grid<int> position(position_.x + width_/2, position_.y + height/2);
      TextProperty no_data(font, "Required more than 2 data.", 16,
                           fore_color_, background_color_, true);
      graph_surface_->drawText(position, no_data, Center);

    } else {
      // !!! ���̏����́A�֐��ɂ��邩�ȁH
      for (std::vector<pImpl::line_t*>::iterator it = draw_lines_.begin();
           it != draw_lines_.end(); ++it) {
        Uint32 color = (*it)->color;

        std::vector<Grid<int> > points;
        for (std::vector<Grid<double> >::iterator pit = (*it)->points.begin();
             pit != (*it)->points.end(); ++pit) {

          Grid<int> graph_point = convertGraphPoint(*pit);
          points.push_back(graph_point);
        }
        graph_surface_->drawLines(points, color);
      }
    }

    created_ = true;
  }

  void registerLine(std::vector<Grid<double> >& points, Uint32 color,
                    TextProperty* text) {

    // �`��͈͂̊g��
    // !!! �ʓr�w�肪�������ꍇ�́A��������D�悷��
    // !!! �����l�����Ȃ���΁A�����������ŃO���t��`�悷�ׂ�����
    for (std::vector<Grid<double> >::iterator it = points.begin();
         it != points.end(); ++it) {
      double x = it->x;
      double y = it->y;

      if (x < x_min_) {
        x_min_ = x;
      }
      if (x > x_max_) {
        x_max_ = x;
      }
      if (y < y_min_) {
        y_min_ = y;
      }
      if (y > y_max_) {
        y_max_ = y;
      }
    }
    line_t* line = new line_t;
    line->points = points;
    line->color = color;
    draw_lines_.push_back(line);
    //fprintf(stderr, "%f, %f, %f, %f\n", x_min_, x_max_, y_min_, y_max_);

    // ���x�����̋L�^
    line_titles_.push_back(text);
    line_colors_.push_back(line->color);

    // �ő�f�[�^���̍X�V
    size_t n = line->points.size();
    if (n > data_max_) {
      data_max_ = n;
    }
  }

  void createLegend(int x, int y, Uint32 fore, Uint32 back) {

    // �����񕝂̕]��
    size_t draw_n = 0;
    size_t font_max = 0;
    size_t width_max = 0;
    size_t height_max = 0;
    for (std::vector<TextProperty*>::iterator it = line_titles_.begin();
         it != line_titles_.end(); ++it) {
      if (*it == NULL) {
        continue;
      }
      ++draw_n;

      // �ő啝�̊m��
      Surface surface(new TextSurface(**it));
      size_t w = surface->getWidth();
      if (w > width_max) {
        width_max = w;
      }
      height_max += surface->getHeight();

      if ((*it)->size > font_max) {
        font_max = (*it)->size;
      }
    }
    if (draw_n == 0) {
      // �`�悷�ׂ������Ȃ��̂ŁA�߂�
      return;
    }

    // �g�̕`��
    Grid<int> position(x, y);
    int legend_width = width_max + LegendLineLength + (TextOffset * 3);
    int legend_height = height_max + TextOffset;
    graph_surface_->drawRect(position, legend_width, legend_height, back);

    // !!! �ȉ��A�֐������ȁH
    Grid<int> p0(x, y);
    Grid<int> p1(x + legend_width, y);
    graph_surface_->drawLine(p0, p1, fore);

    p0.y += legend_height;
    p1.y += legend_height;
    graph_surface_->drawLine(p0, p1, fore);

    p0.x = x;
    p0.y = y;
    p1.x = x;
    p1.y = y + legend_height;
    graph_surface_->drawLine(p0, p1, fore);

    p0.x += legend_width;
    p1.x += legend_width;
    graph_surface_->drawLine(p0, p1, fore);

    int index = 0;
    for (std::vector<TextProperty*>::iterator it = line_titles_.begin();
         it != line_titles_.end(); ++it, ++index) {
      if (*it == NULL) {
        continue;
      }

      // �����̔z�u
      Grid<int> draw_pos(TextOffset + x +  LegendLineLength + TextOffset,
                         TextOffset + y + (font_max * index));
      graph_surface_->drawText(draw_pos, **it, Left);

      // ���̕`��
      draw_pos.x = TextOffset + x;
      draw_pos.y += font_max / 2;
      Grid<int> p(draw_pos.x + LegendLineLength, draw_pos.y);
      graph_surface_->drawLine(draw_pos, p, line_colors_[index]);
    }
  }
};


GraphComponent::GraphComponent(void) : pimpl(new pImpl) {
}


GraphComponent::~GraphComponent(void) {
}


void GraphComponent::addSurfaceList(SurfaceList& surfaces,
                                    const SDL_Rect* area, size_t ticks) {
  pimpl->addSurfaceList(surfaces, area, ticks);
}


void GraphComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void GraphComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t GraphComponent::getWidth(void) {
  return pimpl->width_;
}


size_t GraphComponent::getHeight(void) {
  return pimpl->height_;
}


void GraphComponent::applyInput(const InputEvent& event,
                                std::vector<SDL_Rect>& applied_rects) {

  // !!! ���̐����A���̃`�F�b�N�{�b�N�X�̑���Ƃ����H
}


void GraphComponent::registerLayer(Layer* layer) {
  // !!!
}


void GraphComponent::setBackgroundColor(Uint32 color) {

  pimpl->have_background_ = true;
  pimpl->background_color_ = color;
}


void GraphComponent::setForeColor(Uint32 color) {
  pimpl->fore_color_ = color;
}


void GraphComponent::drawTitle(const TextProperty& title) {

  // �R���|�[�l���g�̍쐬�ƕ`��
  Grid<int> position(pimpl->width_ / 2, pImpl::TextOffset);
  pimpl->graph_surface_->drawText(position, title, Center);
}


void GraphComponent::registerLine(std::vector<Grid<double> >& points,
                                  Uint32 color, TextProperty* text) {
  pimpl->registerLine(points, color, text);
}


void GraphComponent::create(size_t width, size_t height, TtfResource* font) {
  pimpl->create(width, height, font);
}


void GraphComponent::createLegend(int x, int y, Uint32 fore, Uint32 back) {
  pimpl->createLegend(x, y, fore, back);
}


void GraphComponent::clear(void) {

  for (std::vector<pImpl::line_t*>::iterator it = pimpl->draw_lines_.begin();
       it != pimpl->draw_lines_.end(); ++it) {
    delete *it;
  }
  pimpl->draw_lines_.clear();
  pimpl->line_titles_.clear();
  pimpl->line_colors_.clear();

  pimpl->width_ = 0;
  pimpl->height_ = 0;
  set_SdlRect(&pimpl->position_, 0, 0, 0, 0);

  pimpl->data_max_ = 0;
  pimpl->created_ = false;
}
