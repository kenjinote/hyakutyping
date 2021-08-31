/*!
  \file
  \brief ���j���[�R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$

  \todo ���ڂ̊Ԋu��C�ӂ̊Ԋu�ɐݒ�ł���悤�ɂ���
  \todo �c�^�z�u�A���^�z�u���w�肷��΁A�����ŃI�t�Z�b�g�����������悤�ɂ���
*/

#include "MenuComponent.h"
#include "ButtonComponent.h"
#include "GridTypes.h"
#include "SdlUtils.h"
#include "Layer.h"
#include "RedrawSurface.h"
#include "InputEvent.h"
#include "EvaluateArea.h"

using namespace beego;


struct MenuComponent::pImpl {
  enum {
    SurfaceWidth = 0,
  };
  size_t width_;
  size_t height_;
  int selected_index_;
  int decided_index_;

  class MenuItem {
  public:
    boost::shared_ptr<ButtonComponent> button;
    Grid<int> offset;
    SDL_Rect position;

    MenuItem(boost::shared_ptr<ButtonComponent> button_obj,
             int x_offset, int y_offset)
      : button(button_obj), offset(Grid<int>(x_offset, y_offset)) {
      set_SdlRect(&position, 0, 0);
    }
  };
  std::vector<MenuItem> items_;
  SDL_Rect position_;
  bool position_changed_;
  bool key_changed_;
  size_t select_width_;
  Layer* layer_;

  bool icon_placed_;
  Component icon_component_;
  Grid<int> icon_offset_;
  int icon_place_mode_;
  int icon_pre_selected_;
  size_t icon_width_;
  size_t icon_height_;
  SDL_Rect pre_icon_position_;

  DrawSurface draw_surface_;
  boost::shared_ptr<RedrawSurface> redraw_surface_;

  pImpl(void)
    : width_(0), height_(0), selected_index_(NoSelect),
      decided_index_(NoSelect), position_changed_(false), key_changed_(false),
      select_width_(SurfaceWidth), layer_(NULL), icon_placed_(false),
      icon_place_mode_(None), icon_pre_selected_(selected_index_),
      icon_width_(0), icon_height_(0), redraw_surface_(new RedrawSurface) {

    set_SdlRect(&position_, 0, 0, 0, 0);
    set_SdlRect(&pre_icon_position_, 0, 0, 0, 0);
  }

  void updateComponentSize(void) {

    size_t max_width = 0, max_height = 0;
    int x = 0, y = 0;
    for (std::vector<MenuItem>::iterator it = items_.begin();
         it != items_.end(); ++it) {
      size_t button_width = it->button->getWidth();
      int w = x + button_width;
      if (w > static_cast<int>(max_width)) {
        max_width = w;
      }

      size_t button_height = it->button->getHeight();
      int h = y + button_height;
      if (h > static_cast<int>(max_height)) {
        max_height = h;
      }
      set_SdlRect(&it->position, x, y, button_width, button_height);

      x += it->offset.x;
      y += it->offset.y;
    }

    // �A�C�R���̕\���T�C�Y�𔽉f
    // !!!

    width_ = max_width;
    height_ = max_height;
    position_.w = max_width;
    position_.h = max_height;
  }

  void surfaceUpdate(void) {

    // !!! �Ƃ肠�����A�d���͋C�ɂ����쐬����
    int index = 0;
    for (std::vector<MenuItem>::iterator it = items_.begin();
         it != items_.end(); ++it, ++index) {
      boost::shared_ptr<ButtonComponent>& p = it->button;

      if (index == decided_index_) {
        // ��������Ă���ꍇ
        p->showPressedSurface();

      } else if (index == selected_index_) {
        // �I�����T�[�t�F�X
        p->showFocusedSurface();

      } else {
        // �ʏ�T�[�t�F�X
        p->showNormalSurface();
      }
    }
  }

  void setIconComponent(Component icon, int x_offset, int y_offset, int mode) {

    icon_component_ = icon;
    icon_placed_ = true;

    icon_offset_.x = x_offset;
    icon_offset_.y = y_offset;
    icon_place_mode_ = mode;
    icon_width_ = icon_component_->getWidth();
    icon_height_ = icon_component_->getHeight();

    updateComponentSize();
  }

  void addSurfaceList(SurfaceList& surfaces,
                      const SDL_Rect* area, size_t ticks) {
    if (items_.empty()) {
      return;
    }

    SDL_Rect icon_pos;
    int index = 0;
    int x = 0, y = 0;
    for (std::vector<pImpl::MenuItem>::iterator it = items_.begin();
         it != items_.end(); ++it, ++index) {
      boost::shared_ptr<ButtonComponent>& p = it->button;

      SDL_Rect draw_position;
      set_SdlRect(&draw_position, position_.x + x, position_.y + y,
                  p->getWidth(), p->getHeight());
      // !!! �����ŁAposition_changed �� true �ɂȂ�̂ŁA����ĕ`�悳���
      // !!! setPosition ����̂́A
      // !!! �EsetItemsOffset() ���݂̂ł悢�͂�
      // !!! �������AMenuComponent::setPosition() �̂Ƃ��ɂ��A�ʒu�͍X�V�����
      if (position_changed_) {
        p->setPosition(&draw_position);
      }

      x += it->offset.x;
      y += it->offset.y;

      if (index != selected_index_) {
        // �ʏ�T�[�t�F�X
        p->showNormalSurface();

      } else {
        if (index == decided_index_) {
          // ��������Ă���ꍇ
          p->showPressedSurface();
        } else {
          // �I�����T�[�t�F�X
          p->showFocusedSurface();

          if (icon_placed_) {
            SDL_Rect item_pos;
            p->getPosition(&item_pos);
            set_SdlRect(&icon_pos, item_pos.x + icon_offset_.x,
                        item_pos.y + icon_offset_.y, icon_width_, icon_height_);

            size_t item_height = p->getHeight();
            if (icon_place_mode_ & Middle) {
              icon_pos.y += (item_height - icon_height_) / 2;
            }
          }
        }
      }

      p->addSurfaceList(surfaces, area, ticks);
    }

    if (selected_index_ >= 0) {
      if (icon_placed_) {
        // �A�C�R���̔z�u
        if (icon_pre_selected_ != selected_index_) {
          icon_pre_selected_ = selected_index_;

          // �ړ��O�̗̈���N���A
          redraw_surface_->setRedrawRect(pre_icon_position_);
          updateDrawSurface(draw_surface_, redraw_surface_,
                            area, ticks, pre_icon_position_, true);
          surfaces.push_back(&draw_surface_);

          icon_component_->setPosition(&icon_pos);
          pre_icon_position_ = icon_pos;
        }
        icon_component_->addSurfaceList(surfaces, area, ticks);
      }
    }
    position_changed_ = false;
  }
};


MenuComponent::MenuComponent(void) : pimpl(new pImpl) {
}


MenuComponent::~MenuComponent(void) {
}


void MenuComponent::addSurfaceList(SurfaceList& surfaces,
                                   const SDL_Rect* area, size_t ticks) {
  pimpl->addSurfaceList(surfaces, area, ticks);
}


void MenuComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->updateComponentSize();
  pimpl->position_changed_ = true;
}


void MenuComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t MenuComponent::getWidth(void) {
  return pimpl->width_;
}


size_t MenuComponent::getHeight(void) {
  return pimpl->height_;
}


void MenuComponent::applyInput(const InputEvent& event,
                               std::vector<SDL_Rect>& applied_rects) {

  if (pimpl->decided_index_ != NoSelect) {
    // ���ɉ�����ԂȂ�΁A��Ԃ��X�V���Ȃ�
    return;
  }

  // �}�E�X�ʒu�ɂ��鍀�ڂ�I�������ɂ���
  int index = 0;
  int x = 0, y = 0;
  if (event.mouse_moved || (! pimpl->key_changed_)) {
    pimpl->key_changed_ = false;
    for (std::vector<pImpl::MenuItem>::iterator it = pimpl->items_.begin();
         it != pimpl->items_.end(); ++it) {
      boost::shared_ptr<ButtonComponent>& p = it->button;

      // !!! button �� applyInput ��K�p����H
      // !!! ButtonComponent ������������A�Č�������

      SDL_Rect surface_area;
      size_t width = (pimpl->select_width_ == pImpl::SurfaceWidth)
        ? p->getWidth() : pimpl->select_width_;
      set_SdlRect(&surface_area, pimpl->position_.x + x, pimpl->position_.y + y,
                  width, p->getHeight());

      // �}�E�X���A�\���T�[�t�F�X���ɂ���ꍇ�̏���
      // !!! ���ۂɂ͕`�悳��ĂȂ��ꍇ�����肤�邪�A�Ƃ肠�����C�ɂ��Ȃ�
      // !!! �R���|�[�l���g���`��͈͊O�̏ꍇ�Ƃ��A�B��Ă���ꍇ��
      bool cursor_inside = isPointInside(event.mx, event.my, surface_area);
      if (cursor_inside) {
        // �I�����ꂽ
        pimpl->selected_index_ = index;

        if (event.left_released) {
          // ���肳�ꂽ
          pimpl->decided_index_ = index;
        }
        break;
      }

      x += it->offset.x;
      y += it->offset.y;
      ++index;
    }
  }

  // �L�[�{�[�h����ɂ�荀�ڂ�I���A���肷��
  for (std::vector<SDLKey>::const_iterator it = event.key_released.begin();
       it != event.key_released.end(); ++it) {

    int max = pimpl->items_.size() - 1;
    if ((pimpl->selected_index_ > 0) && (*it == SDLK_UP)) {
      // ���
      --pimpl->selected_index_;
      pimpl->key_changed_ = true;

    } else if ((pimpl->selected_index_ < max) &&  (*it == SDLK_DOWN)) {
      // ����
      ++pimpl->selected_index_;
      pimpl->key_changed_ = true;

    } else if (*it == SDLK_LEFT) {
      // ���ň�ԏ�
      pimpl->selected_index_ = 0;
      pimpl->key_changed_ = true;

    } else if (*it == SDLK_RIGHT) {
      // �E�ň�ԉ�
      pimpl->selected_index_ = max;
      pimpl->key_changed_ = true;

    } else if (*it == SDLK_RETURN) {
      // ���^�[���L�[�Ō���
      pimpl->decided_index_ = pimpl->selected_index_;
      break;
    }
  }
}


void MenuComponent::registerLayer(Layer* layer) {
  pimpl->layer_ = layer;

  if (pimpl->icon_placed_ && (pimpl->selected_index_ >= 0)) {
    pimpl->icon_component_->setPosition(&pimpl->pre_icon_position_);
  }
}


void MenuComponent::clearItems(void) {
  pimpl->items_.clear();
}


void MenuComponent::addItem(boost::shared_ptr<ButtonComponent> item,
                            int x_offset, int y_offset) {

  // �{�^���̒ǉ�
  pimpl->items_.push_back(pImpl::MenuItem(item, x_offset, y_offset));

  // width, height �̍Čv�Z
  pimpl->updateComponentSize();
  pimpl->position_changed_ = true;
  // !!!
  // !!! �Ƃ肠�����A�o�^���ꂽ�R���|�[�l���g����Ԃ�
  pimpl->width_ = item->getWidth();
  pimpl->height_ = item->getHeight();
}


void MenuComponent::setItemsOffset(int x_offset, int y_offset) {

  for (std::vector<pImpl::MenuItem>::iterator it = pimpl->items_.begin();
       it != pimpl->items_.end(); ++it) {
    it->offset.x = x_offset;
    it->offset.y = y_offset;
  }

  // width, height �̍Čv�Z
  pimpl->updateComponentSize();
  pimpl->position_changed_ = true;
}


void MenuComponent::getItemPosition(SDL_Rect* offset, size_t index) {
  *offset = pimpl->items_[index].position;
}


void MenuComponent::adjustItemsVerticalOffset(size_t additional_offset) {

  for (std::vector<pImpl::MenuItem>::iterator it = pimpl->items_.begin();
       it != pimpl->items_.end(); ++it) {
    it->offset.y = it->button->getHeight() + additional_offset;
  }

  // width, height �̍Čv�Z
  pimpl->updateComponentSize();
  pimpl->position_changed_ = true;
}


void MenuComponent::adjustItemsHorizontalOffset(size_t additional_offset) {

  for (std::vector<pImpl::MenuItem>::iterator it = pimpl->items_.begin();
       it != pimpl->items_.end(); ++it) {
    it->offset.x = it->button->getWidth() + additional_offset;
  }

  // width, height �̍Čv�Z
  pimpl->updateComponentSize();
  pimpl->position_changed_ = true;
}


void MenuComponent::setSelected(int index) {
  // !!! ���b�N
  releaseDecided();
  pimpl->selected_index_ = index;
  pimpl->surfaceUpdate();

  // !!! �����ŁApre_icon_position_ �̏����������ׂ��C������
}


void MenuComponent::setDecided(int index) {
  // !!! ���b�N
  releaseDecided();
  pimpl->selected_index_ = index;
  pimpl->decided_index_ = index;
  pimpl->surfaceUpdate();
}


int MenuComponent::getSelected(void) {
  return pimpl->selected_index_;
}


int MenuComponent::getDecided(void) {
  return pimpl->decided_index_;
}


void MenuComponent::releaseDecided(void) {
  for (std::vector<pImpl::MenuItem>::iterator it = pimpl->items_.begin();
       it != pimpl->items_.end(); ++it) {
    if (it->button->isDecided()) {
      it->button->releaseDecided();
    }
  }
  pimpl->selected_index_ = pimpl->decided_index_;
  pimpl->decided_index_ = NoSelect;
}


void MenuComponent::setItemSelectWidth(size_t width) {
  pimpl->select_width_ = width;
}


void MenuComponent::setIconComponent(Component icon,
                                     int x_offset, int y_offset, int mode) {
  pimpl->setIconComponent(icon, x_offset, y_offset, mode);
}

