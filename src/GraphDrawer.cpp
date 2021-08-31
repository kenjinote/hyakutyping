/*!
  \file
  \brief �����O���t�̕`��

  \author Satofumi KAMIMURA

  $Id$

  \todo �O���t���A�P�_�ڂ���A������ۂ��\������
  \todo �ŋ߂̃f�[�^�ɂ́A�}�[�J�[��t������
  \todo X ���AY ���Ƀ��x����U��
  \todo �_�������������A�ォ�����A�Ɋ���U��
  \todo �P��ڂ̃f�[�^���͂���A�O���t(�_)���\�������悤�ɂ���
  \todo ���͘a�̐��̕\�����A���f����
*/

#include "GraphDrawer.h"
#include "GraphDrawer_uni.h"
#include "CommonResources.h"
#include "DrawsDefinition.h"
#include "SystemDefinition.h"
#include "AccessSettings.h"
#include "AccessRecordPC.h"
#include "GuiManager.h"
#include "Layer.h"
#include "InputReceiveComponent.h"
#include "InputEvent.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "LabelComponent.h"
#include "GraphComponent.h"
#include "SdlUtils.h"

using namespace beego;


struct GraphDrawer::pImpl {
  TypingMode typing_mode_;
  CommonResources* common_;
  bool is_decided_;
  InputEvent input_event_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  boost::shared_ptr<GraphComponent> graph_;

  pImpl(TypingMode mode)
    : typing_mode_(mode), common_(CommonResources::getObject()),
      is_decided_(false),
      title_property_(common_->font_, GraphDrawer_title,
                      MenuSize, Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      graph_(new GraphComponent) {

    // �R���|�[�l���g�̈ʒu�ݒ�
    SDL_Rect position;
    set_SdlRect(&position, centerPosition(title_label_, 640/2), TextOffset);
    title_label_->setPosition(&position);
  }

  void placeComponents(void) {

    // �O���t�p�̃R���|�[�l���g��z�u
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(graph_);

    is_decided_ = false;
    common_->input_receiver_->clear();
    common_->front_layer_->push_front(common_->user_label_);
  }

  void removeComponents(void) {

    // �O���t�p�R���|�[�l���g�̍폜
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(graph_);

    common_->input_receiver_->clear();
    common_->front_layer_->remove(common_->user_label_);
  }

  bool drawGraph(void) {

    graph_->clear();

    // �L�^�̓ǂݏo��
    AccessRecordPC pc_record(typing_mode_);
    time_t now = time(NULL);
    std::vector<TypingRecorder::GameSettings> data;
    bool ret = pc_record.load(data, common_->getUserName(),
                              0, now, GraphHistoryMax);
    if (ret == false) {
      return false;
    }

    enum {
      DelayLine = 0,
      MissLine,
      SpeedLine,
    };
    int index = 0;
    std::vector<Grid<double> > draw_lines[3];
    for (std::vector<TypingRecorder::GameSettings>::iterator it = data.begin();
         it != data.end(); ++it, ++index) {
      Grid<double> point(index, it->score.miss_types);
      draw_lines[MissLine].push_back(point);

      point.y = it->score.delay_msec / it->waka_num / 1000.0;
      draw_lines[DelayLine].push_back(point);

      point.y = 1000.0 * it->score.types / it->score.elapse_msec;
      draw_lines[SpeedLine].push_back(point);
    }

    // �O���t�ւ̃f�[�^�o�^
    TextProperty miss_title(common_->font_, GraphDrawer_miss,
                            14, Black, White, true);
    TextProperty speed_title(common_->font_, GraphDrawer_speed,
                             14, Black, White, true);
    if (typing_mode_ == ShimonokuTyping) {
      // ���̋�^�C�s���O
      graph_->registerLine(draw_lines[SpeedLine], Blue, &speed_title);
    }
    // ���̋�^�C�s���O�E���܂莚�^�C�s���O
    TextProperty delay_title(common_->font_, GraphDrawer_delay,
                             14, Black, White, true);
    graph_->registerLine(draw_lines[DelayLine], DarkGreen, &delay_title);

    if (typing_mode_ == ShimonokuTyping) {
      // ���̋�^�C�s���O
      graph_->registerLine(draw_lines[MissLine], Red, &miss_title);
    }

    // �O���t�쐬
    graph_->setBackgroundColor(CornSilk);
    graph_->setForeColor(Black);
    graph_->create(GraphWidth, GraphHeight, common_->font_);
    graph_->createLegend(20, 15, Black, 0xf8f8f8ff);

    // �^�C�g���̕`��
    if (typing_mode_ == ShimonokuTyping) {
      // ���̋�^�C�s���O
      TextProperty shimonoku_property(common_->font_, GraphDrawer_shimonoku,
                                      SmallSize, Fore, Back, true);
      graph_->drawTitle(shimonoku_property);

    } else {
      // ���܂莚�^�C�s���O
      TextProperty kimariji_property(common_->font_, GraphDrawer_kimariji,
                                     SmallSize, Fore, Back, true);
      graph_->drawTitle(kimariji_property);
    }

    SDL_Rect position;
    set_SdlRect(&position, centerPosition(graph_, 640/2),
                middlePosition(graph_, (MenuSize + 480) / 2));
    graph_->setPosition(&position);

    return true;
  }
};


GraphDrawer::GraphDrawer(TypingMode mode) : pimpl(new pImpl(mode)) {
}


GraphDrawer::~GraphDrawer(void) {
}


bool GraphDrawer::drawGraph(void) {
  return pimpl->drawGraph();
}


void GraphDrawer::placeComponents(void) {
  pimpl->placeComponents();
}


void GraphDrawer::removeComponents(void) {
  pimpl->removeComponents();
}


bool GraphDrawer::keyPressed(void) {

  pimpl->common_->input_receiver_->updateInputEvent(pimpl->input_event_);
  pimpl->is_decided_ |=
    InputEvent::isReleased(pimpl->input_event_, SDLK_RETURN);

  return pimpl->is_decided_;
}
