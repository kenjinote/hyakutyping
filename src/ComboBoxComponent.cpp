/*!
  \file
  \brief �R���{�{�b�N�X�E�R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$

  \todo ���O�p�̉摜�����ւ�����悤�ɂ���
  \todo �J�[�\�������j���[�O�̂Ƃ��A�I�����j���[���ڂ��I������Ȃ��悤�ɂ���
  \todo �Ȃ񂩁A�I�����ڂ̓���ւ��`�悪�������B�������m�F���邠���肩��
*/

#include "ComboBoxComponent.h"
#include "down_triangle.h"
#include "Layer.h"
#include "MenuComponent.h"
#include "ButtonComponent.h"
#include "Component.h"
#include "InputEvent.h"
#include "TextProperty.h"
#include "SdlSurface.h"
#include "ColorSurface.h"
#include "CreateSurfaceFromArray.h"
#include "SwitchSurface.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"

using namespace beego;


struct ComboBoxComponent::pImpl {

  enum {
    DrawDecidedBack = 0,        //!< �S�̘̂g
    DrawDecided,                //!< �I�����ڂ̘g
    DrawDecidedItemBack,        //!< �I�����ڂ̔w�i�g
    DrawTriangle,               //!< ���O�p
    DrawMenuBack,               //!< �I�����j���[�̔w�i
    DrawMenu,                   //!< �I�����j���[
    DrawIndexMax,               //!< ��L�v�f�̌�

    DrawSelectMenuBack,         //!< �I�����j���[���̑I�����ڂ̔w�i
  };

  enum {
    Normal = 0,
    Selected,
  };

  size_t max_width_;
  boost::shared_ptr<SwitchSurface> decided_surface_;
  boost::shared_ptr<MenuComponent> menu_;
  SDL_Rect position_;
  SDL_Rect menu_position_;
  bool position_changed_;
  bool show_;                 //!< �I�����j���[���J���Ă���Ƃ��� true
  Uint32 frame_color_;
  Uint32 decided_back_color_;
  Uint32 selected_back_color_;
  Surface triangle_surface_;
  Surface decided_back_;
  Surface decided_item_back_;
  Surface menu_back_;
  typedef std::vector<boost::shared_ptr<SwitchSurface> > SwitchSurfaces;
  SwitchSurfaces selected_items_back_;
  DrawSurface draw_surfaces_[DrawIndexMax];
  std::vector<DrawSurface> draw_surfaces_items_back_;
  Layer* layer_;
  bool is_select_changed_;
  size_t min_width_;
  size_t right_offset_;

  pImpl(void)
    : max_width_(0),
      decided_surface_(new SwitchSurface), menu_(new MenuComponent),
      position_changed_(false), show_(false),
      frame_color_(Gray12), decided_back_color_(White),
      selected_back_color_(Blue),
      triangle_surface_(new SdlSurface(createSurface(down_triangle,
                                                     down_triangle_width,
                                                     down_triangle_height),
                                       true)),
      layer_(NULL), is_select_changed_(false),
      min_width_(0), right_offset_(0) {

    set_SdlRect(&position_, 0, 0, 0, 0);
    updateBackSurface();
  }

  void updateBackSurface(void) {

    // �I���T�[�t�F�X�p�́A�S�̂̔w�i�摜���쐬
    size_t triangle_width = triangle_surface_->getWidth();
    size_t frame_width = max_width_ + right_offset_ + triangle_width;
    size_t decided_height = decided_surface_->getHeight();
    Surface new_decided_back(new ColorSurface(frame_width, decided_height,
                                              frame_color_));
    std::swap(decided_back_, new_decided_back);

    // �I���T�[�t�F�X�p�́A���ڂ̔w�i�摜���쐬
    Surface new_item_back(new ColorSurface(max_width_, decided_height,
                                           decided_back_color_));
    std::swap(decided_item_back_, new_item_back);

    // �I�����j���[���́A�I�����̔w�i�T�[�t�F�X�̍쐬
    size_t index = 0;

    // !!! selected_items_back_ �̓v�[���ł��鎑���B��蒼���Ă��悢
    selected_items_back_.clear();
    for (std::vector<DrawSurface>::iterator it =
           draw_surfaces_items_back_.begin();
         it != draw_surfaces_items_back_.end(); ++it, ++index) {

      boost::shared_ptr<SwitchSurface> items_back(new SwitchSurface());
      SDL_Rect item_offset;
      menu_->getItemPosition(&item_offset, index);
      size_t height = item_offset.h;
      Surface normal_back(new ColorSurface(frame_width, height,
                                           decided_back_color_));
      Surface selected_back(new ColorSurface(frame_width, height,
                                             selected_back_color_));
      items_back->registerSurface(normal_back, 0, 0, pImpl::Normal);
      items_back->registerSurface(selected_back, 0, 0, pImpl::Selected);
      selected_items_back_.push_back(items_back);
    }

    // ���j���[�p�̔w�i�摜���쐬
    Surface new_menu_back(new ColorSurface(frame_width, menu_->getHeight(),
                                           decided_back_color_));
    std::swap(menu_back_, new_menu_back);
  }

  size_t getWidth(void) {
    return max_width_ + right_offset_ + triangle_surface_->getWidth();
  }

  size_t getHeight(void) {

    // ���j���[���J���Ă��邩�ۂ��ō����͕ς��
    size_t height =
      decided_back_->getHeight() + ((show_) ? menu_->getHeight() : 0);

    return height;
  }

  void addSurfaceList(SurfaceList& surfaces,
                      const SDL_Rect* area, size_t ticks) {

    // !!! ������̓o�^�́Afor �ōs���悤�ɏC������
    // !!! �������A�ʒu�̓o�^��������Ń��[�v������̂́A�ʓ|����

    SDL_Rect draw_area;

    // �I�����ڂ̔w�i
    size_t decided_item_back_width =
      decided_item_back_->getWidth() + right_offset_;
    size_t decided_item_back_height = decided_item_back_->getHeight();

    // ���O�p
    size_t triangle_width = triangle_surface_->getWidth();
    size_t triangle_height = triangle_surface_->getHeight();
    size_t frame_width = decided_item_back_width + triangle_width;
    set_SdlRect(&draw_area,
                position_.x + frame_width - triangle_width,
                position_.y + (decided_item_back_height - triangle_height)/2,
                triangle_width, triangle_height);
    updateDrawSurface(draw_surfaces_[pImpl::DrawTriangle],
                      triangle_surface_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[pImpl::DrawTriangle]);

    // �I���T�[�t�F�X
    set_SdlRect(&draw_area, position_.x, position_.y,
                decided_surface_->getWidth(),
                decided_surface_->getHeight());
    updateDrawSurface(draw_surfaces_[pImpl::DrawDecided],
                      decided_surface_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[pImpl::DrawDecided]);

    // �I�����ڂ̔w�i
    set_SdlRect(&draw_area, position_.x, position_.y,
                decided_item_back_width, decided_item_back_height);
    updateDrawSurface(draw_surfaces_[pImpl::DrawDecidedItemBack],
                      decided_item_back_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[pImpl::DrawDecidedItemBack]);

    // �I���T�[�t�F�X�w�i
    set_SdlRect(&draw_area, position_.x, position_.y,
                decided_back_->getWidth(),
                decided_back_->getHeight());
    updateDrawSurface(draw_surfaces_[pImpl::DrawDecidedBack],
                      decided_back_, area, ticks,
                      draw_area, position_changed_);
    surfaces.push_back(&draw_surfaces_[pImpl::DrawDecidedBack]);

    if (show_) {
      // �I�����j���[ �̕`��
      menu_->addSurfaceList(surfaces, area, ticks);

      // �I�����j���[�̑I������
      size_t index = 0;
      SDL_Rect item_offset;
      size_t now_selected = menu_->getSelected();
      for (SwitchSurfaces::iterator it = selected_items_back_.begin();
           it != selected_items_back_.end(); ++it, ++index) {

        menu_->getItemPosition(&item_offset, index);
        if (index == now_selected) {
          // �I���T�[�t�F�X
          (*it)->switchSurface(Selected);

        } else {
          // �ʏ�T�[�t�F�X
          (*it)->switchSurface(Normal);
        }
        set_SdlRect(&draw_area,
                    position_.x + item_offset.x,
                    position_.y + item_offset.y + decided_item_back_height,
                    decided_item_back_width + triangle_width,
                    item_offset.h);
        updateDrawSurface(draw_surfaces_items_back_[index],
                          *it, area, ticks,
                          draw_area, position_changed_);
        surfaces.push_back(&draw_surfaces_items_back_[index]);
      }

      // �I�����j���[�w�i �̕`��
      set_SdlRect(&draw_area, position_.x,
                  position_.y + decided_item_back_height,
                  menu_back_->getWidth(),
                  menu_back_->getHeight());
      updateDrawSurface(draw_surfaces_[pImpl::DrawMenuBack],
                        menu_back_, area, ticks,
                        draw_area, position_changed_);
      surfaces.push_back(&draw_surfaces_[pImpl::DrawMenuBack]);
    }
    position_changed_ = false;
  }
};


ComboBoxComponent::ComboBoxComponent(void) : pimpl(new pImpl) {
}


ComboBoxComponent::~ComboBoxComponent(void) {
}


void ComboBoxComponent::addSurfaceList(SurfaceList& surfaces,
                                       const SDL_Rect* area, size_t ticks) {

  pimpl->addSurfaceList(surfaces, area, ticks);
}


void ComboBoxComponent::setPosition(const SDL_Rect* position) {
  set_SdlRect(&pimpl->position_, position->x, position->y,
              pimpl->position_.w, pimpl->position_.h);
  pimpl->position_changed_ = true;
}


void ComboBoxComponent::getPosition(SDL_Rect* position) {
  *position = pimpl->position_;
}


size_t ComboBoxComponent::getWidth(void) {
  return pimpl->getWidth();
}


size_t ComboBoxComponent::getHeight(void) {
  return pimpl->getHeight();
}


void ComboBoxComponent::applyInput(const InputEvent& event,
                                   std::vector<SDL_Rect>& applied_rects) {

  if (pimpl->show_) {
    // �I�����j���[���J����Ă���Ƃ�

    // �I������
    pimpl->menu_->applyInput(event, applied_rects);

    if (event.left_released) {
      // �I�����j���[�̍폜
      SurfaceList remove_surfaces;
      for (std::vector<DrawSurface>::iterator it =
             pimpl->draw_surfaces_items_back_.begin();
           it != pimpl->draw_surfaces_items_back_.end(); ++it) {
        remove_surfaces.push_back(&(*it));
      }

      pimpl->menu_->addSurfaceList(remove_surfaces, &pimpl->menu_position_, 0);
      remove_surfaces.push_back(&pimpl->draw_surfaces_[pImpl::DrawMenuBack]);
      pimpl->layer_->removeSurfaces(remove_surfaces);
      pimpl->show_ = false;

      // �I�����ڂ̍Đݒ�
      int decided_index = pimpl->menu_->getDecided();
      if (decided_index >= 0) {
        pimpl->is_select_changed_ = true;
        pimpl->menu_->releaseDecided();
        pimpl->menu_->setDecided(MenuComponent::NoSelect);
        pimpl->decided_surface_->switchSurface(decided_index);
        pimpl->updateBackSurface();
        pimpl->menu_back_->forceSetChanged();
      }
    }

  } else {
    // �I�����j���[���J����Ă��Ȃ��Ƃ�

    // �g�����N���b�N���ꂽ���A�̔���
    SDL_Rect component_area;
    size_t decided_item_height = pimpl->decided_item_back_->getHeight();
    set_SdlRect(&component_area, pimpl->position_.x, pimpl->position_.y,
                pimpl->decided_back_->getWidth(),
                pimpl->decided_back_->getHeight());
    bool cursor_inside = isPointInside(event.mx, event.my, component_area);
    if (cursor_inside && event.left_released) {
      // �I�����j���[���J��

      // �z�u�ʒu�̍X�V�B�ʒu���X�V���邱�ƂŁA�`�悪�ۏ؂����
      set_SdlRect(&pimpl->menu_position_, pimpl->position_.x,
                  pimpl->position_.y + decided_item_height,
                  pimpl->menu_->getWidth(), pimpl->menu_->getHeight());
      pimpl->menu_->setPosition(&pimpl->menu_position_);
      pimpl->menu_back_->forceSetChanged();
      pimpl->show_ = true;
    }
  }
}


void ComboBoxComponent::registerLayer(Layer* layer) {
  pimpl->layer_ = layer;
}


void ComboBoxComponent::setRightOffset(size_t width) {
  pimpl->right_offset_ = width;
}


void ComboBoxComponent::setMinWidth(size_t width) {
  pimpl->min_width_ = width;
}


void ComboBoxComponent::clearItems(void) {

  // �T�C�Y�̏�����
  pimpl->max_width_ = 0;

  // �I�����T�[�t�F�X�̃N���A
  pimpl->decided_surface_->clear();
  pimpl->selected_items_back_.clear();
  pimpl->draw_surfaces_items_back_.clear();

  // ���j���[�̃N���A
  pimpl->menu_->clearItems();
}


void ComboBoxComponent::addItem(boost::shared_ptr<ButtonComponent> item) {

  // �ǂ̃^�C�~���O�� setMinWidth() ���Ă΂�邩�킩��Ȃ��̂ŁA�����ŏ�������
  if (pimpl->max_width_ < pimpl->min_width_) {
    pimpl->max_width_ = pimpl->min_width_;
  }

  // �T�C�Y�̊g��
  size_t item_width = item->getWidth();
  if (item_width > pimpl->max_width_) {
    pimpl->max_width_ = item_width;
  }
  pimpl->menu_->setItemSelectWidth(pimpl->max_width_ + pimpl->right_offset_
                                   + pimpl->triangle_surface_->getWidth());
  DrawSurface dummy;
  pimpl->draw_surfaces_items_back_.push_back(dummy);

  // ���j���[�ւ̓o�^
  pimpl->menu_->addItem(item);
  pimpl->menu_->adjustItemsVerticalOffset();

  // �I���T�[�t�F�X�ւ̓o�^�B�T�[�t�F�X�� ButtonComponent ���甲���o��
  Surface normal_surface = item->getNormalSurface();

  // ���݂̓o�^�T�[�t�F�X���̎擾�Ɗi�[
  size_t decided_size = pimpl->decided_surface_->size();
  pimpl->decided_surface_->registerSurface(normal_surface, 0, 0, decided_size);

  // �ŏ��̓o�^�̏ꍇ�A�I�����ڂƂ��ēo�^
  if (decided_size == 0) {
    setSelected(0);
  }

  // �w�i�摜���̍�蒼��
  pimpl->updateBackSurface();
}


void ComboBoxComponent::setSelected(int index) {
  pimpl->decided_surface_->switchSurface(index);
  pimpl->updateBackSurface();
}


int ComboBoxComponent::getSelected(void) {
  if (pimpl->decided_surface_->size() == 0) {
    return NoItem;
  }
  return pimpl->decided_surface_->getSurfaceIndex();
}


bool ComboBoxComponent::isSelectChanged(void) {

  bool ret = pimpl->is_select_changed_;
  pimpl->is_select_changed_ = false;
  return ret;
}


void ComboBoxComponent::setFrameColor(Uint32 color) {
  pimpl->frame_color_ = color;
}


void ComboBoxComponent::setDecidedBackgroundColor(Uint32 color) {
  pimpl->decided_back_color_ = color;
}


void ComboBoxComponent::setSelectedBackgroundColor(Uint32 color) {
  pimpl->selected_back_color_ = color;
}


// �w�蕶���񂩂�Ȃ�R���{�{�b�N�X���쐬����
void ComboBoxComponent::createComboBox(boost::shared_ptr<ComboBoxComponent>&
                                       combobox,
                                       std::vector<std::string>& texts,
                                       const TextProperty& normal_property,
                                       const TextProperty& decided_property,
                                       Uint32 frame_color) {
  // �o�^���e���N���A
  combobox->clearItems();

  // �F�̐ݒ�
  combobox->setFrameColor(frame_color);
  combobox->setDecidedBackgroundColor(normal_property.back_color);
  combobox->setSelectedBackgroundColor(decided_property.back_color);

  // �{�^���̍쐬�Ɠo�^
  TextProperty normal = normal_property;
  TextProperty decided = decided_property;
  for (std::vector<std::string>::iterator it = texts.begin();
       it != texts.end(); ++it) {

    normal.text = it->c_str();
    decided.text = it->c_str();

    // ���ڂ̓o�^
    boost::shared_ptr<ButtonComponent> new_button(new ButtonComponent);
    ButtonComponent::createButton(new_button, normal, decided, decided);
    combobox->addItem(new_button);
  }
}
