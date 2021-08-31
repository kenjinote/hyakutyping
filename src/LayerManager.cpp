/*!
  \file
  \brief ���C���[�Ǘ�

  \author Satofumi KAMIMURA

  $Id$

  \todo LogManager �܂�肪 SdlSurface �Ɠ����Ȃ�΂�����
  \todo pimpl ���V���O���g���ɂ��ׂ�

  \attention �Ȃ񂩁A�����̃R���|�[�l���g�ԂŔ�r���s���āA���������������B��������A����
*/

#include "LayerManager.h"
#include "Layer.h"
#include "SurfaceInterface.h"
#include "ComponentInterface.h"
#include "GuiColors.h"
#include "EvaluateArea.h"
#include "SdlUtils.h"
#include "LogManager.h"
#include <string>

using namespace beego;


struct LayerManager::pImpl {
  typedef std::vector<Layer*> LayerList;
  SurfaceList& surface_list;
  LayerList layer_list;
  SDL_Surface* scr;
  Uint32 back_color;
  size_t width;
  size_t height;
  typedef std::vector<SDL_Rect> Rects;
  Rects back_rects;
  Rects remove_update_rects;

  pImpl(SurfaceList& surfaces)
    : surface_list(surfaces), scr(SDL_GetVideoSurface()), width(0), height(0) {
#if 0
    if (scr == NULL) {
      LogManager* log = LogManager::getObject();
      std::string message =
        "LayerManager::pImpl(): Screen surface must be created.";
      log->write(LogManager::Warning, message.c_str());
      return;
    }
#endif
    updateScreenInfo();
  }

  void updateScreenInfo(void) {
    if (scr) {
      width = scr->w;
      height = scr->h;
      back_color = getSdlColor(scr, Black);
    }
  }
};


LayerManager::LayerManager(SurfaceList& surface_list)
  : pimpl(new pImpl(surface_list)) {
}


LayerManager::~LayerManager(void) {
}


void LayerManager::updateScreenSurface(SDL_Surface* scr) {
  pimpl->scr = scr;
  pimpl->updateScreenInfo();
}


void LayerManager::applyInput(ComponentList& components,
                              const InputEvent& event,
                              std::vector<SDL_Rect>& applied_rects) {

  SDL_Rect component_rect;
  for (ComponentList::iterator it = components.begin();
       it != components.end(); ++it) {
    (*it)->applyInput(event, applied_rects);

    // ���͔�����s�����̈��o�^
    (*it)->getPosition(&component_rect);
    component_rect.w = static_cast<int>((*it)->getWidth());
    component_rect.h = static_cast<int>((*it)->getHeight());
    applied_rects.push_back(component_rect);
  }
}


void LayerManager::redraw(std::vector<SDL_Rect>& update_rects,
                          SurfaceList& surfaces) {

  if (! pimpl->scr) {
    return;
  }

  // �폜�T�[�t�F�X���� update_rects ���󂯎��
  // !!! �{���́Ainsert �ɂ���ׂ��B������C������
  update_rects = pimpl->remove_update_rects;
  pimpl->remove_update_rects.clear();

  // �w�i�F�ł̓h��Ԃ�
  //fprintf(stderr, "back_rects: %d\n", pimpl->back_rects.size());
  for (pImpl::Rects::iterator it = pimpl->back_rects.begin();
       it != pimpl->back_rects.end(); ++it) {
    SDL_FillRect(pimpl->scr, &*it, pimpl->back_color);
    update_rects.push_back(*it);
  }
  pimpl->back_rects.clear();

  for (SurfaceList::reverse_iterator rit = surfaces.rbegin();
       rit != surfaces.rend(); ++rit) {
    bool is_changed = (*rit)->is_changed | (*rit)->position_changed;

    if (is_changed) {
      //SDL_Rect& rect = (*rit)->position;
      //fprintf(stderr, "draw!!! (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
      (*rit)->surface->draw(update_rects, &(*rit)->position, &(*rit)->area);

      // !!! ����� createSurfaceList() �]�����ɍ폜�����̂�h������
      (*rit)->is_changed = false;
      continue;
    }
  }
}


void LayerManager::push_front(Layer* layer) {
  pimpl->layer_list.insert(pimpl->layer_list.begin(), layer);
  layer->registerManager(this);

  // !!! ���̂Ƃ��ɁASurfaceList �̍X�V���s��
}


void LayerManager::push_back(Layer* layer) {

  pimpl->layer_list.push_back(layer);
  layer->registerManager(this);

  // !!! ���̂Ƃ��ɁASurfaceList �̍X�V���s��
}


void LayerManager::insert(const Layer* dst, Layer* layer) {

  pImpl::LayerList::iterator it = find(pimpl->layer_list.begin(),
                                       pimpl->layer_list.end(), dst);
  pimpl->layer_list.insert(it, layer);
  layer->registerManager(this);

  // !!! ���̂Ƃ��ɁASurfaceList �̍X�V���s��
}


void LayerManager::remove(Layer* layer) {

  pImpl::LayerList::iterator it = find(pimpl->layer_list.begin(),
                                       pimpl->layer_list.end(), layer);
  if (it != pimpl->layer_list.end()) {
    pimpl->layer_list.erase(it);
  }
}


void LayerManager::createComponentList(ComponentList& components) {

  components.clear();

  for (pImpl::LayerList::iterator it = pimpl->layer_list.begin();
       it != pimpl->layer_list.end(); ++it) {
    (*it)->addComponentList(components);
  }
}


void LayerManager::createSurfaceList(SurfaceList& surfaces,
                                     const ComponentList& components,
                                     size_t ticks) {

  // ���̎��_�ł́A�O��ɍĕ`�悵�����̃T�[�t�F�X���X�g���ێ�����Ă���

  //fprintf(stderr, "createSurfaceList: %d\n", surfaces.size());

  // �ύX���ꂽ�T�[�t�F�X�͂����Ŏ�菜���A���߂čĕ`����s��
  // !!! ���߂Ă̍ĕ`��́A���̎��� redraw() �ōs����͂�
  SurfaceList changed_surfaces;
  for (SurfaceList::iterator it = surfaces.begin();
       it != surfaces.end(); ++it) {

    // !!! ���̂�����A�ʒu�̕ω������o���ĂȂ��̂́A���Ȃ��̂��ȁH

    // !!! �ςȎd�g�݂���...
    if ((*it)->surface->isChanged(ticks)) {
      //fprintf(stderr, "is_changed !\n");
      // redraw() �ŕ`�悳���悤�ɁA�t���O���Z�b�g���Ă���
      (*it)->surface->forceSetChanged();
      changed_surfaces.push_back(*it);
    }
  }
  //fprintf(stderr, "changed_surfaces: %d\n", changed_surfaces.size());
  removeSurface(changed_surfaces);

  // �폜�̈�̍X�V
  // !!! �O��̕`�悩��ύX���ꂽ�ӏ����A�w�ʂɂ���T�[�t�F�X�ŕ`�悷��

  // !!! �������ɁA���̂�����͐�p�̊֐����ɂ��Ă��܂��܂��傤

  // !!! �Ƃ肠�����폜�͈̔͂ň�x�`�悵�Ă��܂�
  // !!! �{���́A�ĕ`��͍Ō�ɂ܂Ƃ߂čs���΂悢�͂�

  for (SurfaceList::iterator it = surfaces.begin();
       it != surfaces.end(); ++it) {
    pImpl::Rects& redraw_rects = (*it)->redraw_rects;
#if 0
    if (! redraw_rects.empty()) {
      fprintf(stderr, "redraw_rects: %d\n", redraw_rects.size());
    }
#endif
    for (pImpl::Rects::iterator draw_it = redraw_rects.begin();
         draw_it != redraw_rects.end(); ++draw_it) {
      SDL_Rect draw_area;
      set_SdlRect(&draw_area, draw_it->x - (*it)->position.x,
                  draw_it->y - (*it)->position.y, draw_it->w, draw_it->h);
      //fprintf(stderr, "redraw_rect: %d, %d, %d, %d\n", draw_area.x, draw_area.y, draw_area.w, draw_area.h);
#if 0
      SDL_FillRect(pimpl->scr, &draw_area, pimpl->back_color);
#else
      (*it)->surface->draw(pimpl->remove_update_rects, &*draw_it, &draw_area);
#endif
    }
    redraw_rects.clear();
  }
  // !!! ��L���֐�������


  surfaces.clear();
  SDL_Rect area;
  set_SdlRect(&area, 0, 0,
              static_cast<int>(pimpl->width -1),
              static_cast<int>(pimpl->height -1));

  for (ComponentList::const_iterator it = components.begin();
       it != components.end(); ++it) {
    (*it)->addSurfaceList(surfaces, &area, ticks);
  }
}


void LayerManager::removeSurface(SurfaceList& remove_list) {

  // ��O����폜�T�[�t�F�X�܂ŁA�ĕ`��̈��]������

  // !!! �Ƃ肠�����A�S��ʂɑ΂��ĕ]�����s��
  // !!! ��ʕ��́A�{���͂����Ǝ擾���ׂ�
  SDL_Rect all_area;
  set_SdlRect(&all_area, 0, 0, 640, 480);
  pImpl::Rects redraw_areas;
  redraw_areas.push_back(all_area);

  // ������̈�
  for (pImpl::Rects::iterator redraw_it = redraw_areas.begin();
       redraw_it != redraw_areas.end(); ++redraw_it) {

    // �폜�T�[�t�F�X
    // !!! �����ŁA�폜����T�[�t�F�X�ԂŁA�d�Ȃ�̈��]�����Ă��������悳����
    for (SurfaceList::iterator remove_it = remove_list.begin();
         remove_it != remove_list.end(); ++remove_it) {

      //fprintf(stderr, "%d, %d, %d, %d\n", (*remove_it)->position.x, (*remove_it)->position.y, (*remove_it)->area.w, (*remove_it)->area.h);

      pImpl::Rects remove_area;
      add_SdlRect(remove_area,
                  (*remove_it)->position.x, (*remove_it)->position.y,
                  (*remove_it)->area.w, (*remove_it)->area.h);

      pImpl::Rects common_area; // �g��Ȃ�
      pImpl::Rects unique_area; // �폜����T�[�t�F�X�̌����Ă��镔��
      bool front_surface = true;

      // �Ǘ����Ă���T�[�t�F�X
      for (SurfaceList::iterator surface_it = pimpl->surface_list.begin();
           surface_it != pimpl->surface_list.end();) {
        //fprintf(stderr, "cmp: %p\n", *surface_it);
        SDL_Rect surface_area;
        set_SdlRect(&surface_area,
                    (*surface_it)->position.x, (*surface_it)->position.y,
                    (*surface_it)->area.w, (*surface_it)->area.h);

        if (front_surface) {
          // �O�ʃT�[�t�F�X�Ƃ̕]��
          // �����Ă���̈�� remove_area �ɓo�^���Ă���

          if (*surface_it == *remove_it) {
            //fprintf(stderr, "match equal.\n");

            // ���T�[�t�F�X�܂ŕ]��������A���͔w�ʂ̕]�����s��
            front_surface = false;

            // ���T�[�t�F�X�͍폜����邽�߁A�o�^����Ă���ĕ`��̈�͎��ɓn��
            // !!! ���̏����A�Ӗ�����̂��ȁH
            for (pImpl::Rects::iterator it =
                   (*surface_it)->redraw_rects.begin();
                 it != (*surface_it)->redraw_rects.end(); ++it) {
              remove_area.push_back(*it);
            }
            (*surface_it)->redraw_rects.clear();

            // ���T�[�t�F�X�̓o�^���폜
            surface_it = pimpl->surface_list.erase(surface_it);
            // !!! ���� -- �́A�ς���Ȃ����H
            //--surface_it;
            // !!! surface_it �� front() �̂Ƃ��ɖ�肪���肻��
            // !!! �܂��A���[�v�� ++ ���Ă邩������I�Ȗ��͂Ȃ��Ǝv�����ǂ�...

          } else {
            // �폜����T�[�t�F�X�̌����Ă��镔���� remove_area �ɓo�^����
            unique_area.clear();
            common_area.clear();
            for (pImpl::Rects::iterator unique_it = remove_area.begin();
                 unique_it != remove_area.end(); ++unique_it) {

              evaluateArea(common_area, unique_area, *unique_it, surface_area);

              // !!! �ȍ~�A�֐������Ă�����B������
              // ���߃T�[�t�F�X�̂Ƃ��́Acommon ���ȍ~�̃T�[�t�F�X�ɓn��
              if ((*surface_it)->is_transparent) {
                unique_area.insert(unique_area.end(),
                                   common_area.begin(), common_area.end());
              }
            }
            remove_area = unique_area;
            ++surface_it;
          }
        } else {
          // !!! ���� else �́A�Q�̊֐��ɂ��Ă��܂���������

          // �w�ʃT�[�t�F�X�Ƃ̕]��
          // remove_area �̗̈���A�w�ʂɂ���T�[�t�F�X�ɍĕ`�悳����

          common_area.clear();
          unique_area.clear();
          for (pImpl::Rects::iterator it = remove_area.begin();
               it != remove_area.end(); ++it) {

            // �d�Ȃ蔻����s���Acommon �����̃T�[�t�F�X
            // unique �����̃T�[�t�F�X��
            evaluateArea(common_area, unique_area, *it, surface_area);
            //fprintf(stderr, "(%d, %d, %d, %d) (%d, %d, %d, %d) ", it->x, it->y, it->w, it->h, surface_area.x, surface_area.y, surface_area.w, surface_area.h);
            //fprintf(stderr, "common: %d, unique: %d\n", common_area.size(), unique_area.size());
            pImpl::Rects::iterator last_it = (*surface_it)->redraw_rects.end();
            (*surface_it)->redraw_rects.insert(last_it, common_area.begin(),
                                               common_area.end());
            common_area.clear();

            // ���߃T�[�t�F�X�̂Ƃ��́Acommon ���ȍ~�̃T�[�t�F�X�ɓn��
            if ((*surface_it)->is_transparent) {
              unique_area.insert(unique_area.end(),
                                 common_area.begin(), common_area.end());
            }
          }
          remove_area = unique_area;
          ++surface_it;
        }
      }
      // remove_area ��w�i�F�œh��Ԃ��̈�Ƃ��āA�ĕ`��̈�ɓo�^����
      for (pImpl::Rects::iterator it = remove_area.begin();
           it != remove_area.end(); ++it) {
        pimpl->back_rects.push_back(*it);
      }
    }
  }
}
