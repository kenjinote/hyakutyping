/*!
  \file
  \brief ���̋�̓��͗��K

  \author Satofumi KAMIMURA

  $Id$

  \todo ���܂莚�A���킩��悤�ɂ��ĕ\������
  \todo ���͌�́A�����Ɏc��̋��\������
  \todo �ŏ��́uX ��v�̕\�����ז��H
  \todo �L�[�����\������
  \todo �����I�ɂ́ACD-ROM �� �T�� / �T���Ȃ� ���I�v�V�����őI���ł���悤�ɂ��ׂ�
*/

#include "ShimonokuPractice.h"
#include "ShimonokuPractice_uni.h"
#include "PracticeMenu_uni.h"
#include "DrawsDefinition.h"
#include "SystemDefinition.h"
#include "ResourceDefinition.h"
#include "ShimonokuInput.h"
#include "CommonResources.h"
#include "ResultDrawer.h"
#include "GraphDrawer.h"
#include "TypingRecorder.h"
#include "ShuffleWaka.h"
#include "VoicePlayer.h"
#include "GuiManager.h"
#include "Layer.h"
#include "BaseEntity.h"
#include "StateMachine.h"
#include "GetTicks.h"
#include "Delay.h"
#include "TextSurface.h"
#include "TextProperty.h"
#include "LabelComponent.h"
#include "SdlUtils.h"
#include "InputHandler.h"
#include "InputEvent.h"
#include "InputReceiveComponent.h"
#include "UtfString.h"
#include <time.h>

using namespace beego;


struct ShimonokuPractice::pImpl {

  class Schedule : public BaseEntity {
    bool is_terminated_;
    StateMachine<Schedule>* state_machine_;

  public:
    Schedule(int id, CommonResources* common)
      : BaseEntity(id), is_terminated_(false) {
      state_machine_ = new StateMachine<Schedule>(this);
      state_machine_->setCurrentState(FirstState::getObject(common));
    }

    ~Schedule(void) {
      delete state_machine_;
    }

    void update(void) {
      state_machine_->update();
    }

    void changeState(State<Schedule>* new_state) {
      state_machine_->changeState(new_state);
    }

    void setTerminate(void) {
      is_terminated_ = true;
    }

    bool isTerminated(void) {
      return is_terminated_;
    }
  };

  class FirstState : public State<Schedule> {
    CommonResources* common_;

    FirstState(CommonResources* common) : common_(common) {
    }

  public:
    static FirstState* getObject(CommonResources* common) {
      static FirstState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      type->setTerminate();
    }

    void execute(Schedule* type) {
      type->changeState(PrologueState::getObject(common_));
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      // �Q�[�����̋L�^
      int rand_seed = static_cast<int>(time(NULL));
      //rand_seed = ShuffleWaka::NoShuffleSeed;

      // �a�̂̏��Ԃ��V���b�t��
      ShuffleWaka wakaShuffle(ShimonokuTyping, rand_seed);
      int waka_no = common_->getPracticeWakaNo();
      if (waka_no >= 0) {
        // �d�_���K�̎w��𔽉f
        wakaShuffle.setSomeWaka(waka_no, common_->getPracticeNum());
      }
      wakaShuffle.shuffle(common_->waka_order_);

      // !!! �d�_���K�܂��̎���
      // !!!
      // wakaShuffle.shuffle();

      common_->recorder_->recordGame(rand_seed, ShimonokuTyping);
    }
  };

  class PrologueState : public State<Schedule> {

    CommonResources* common_;
    // !!! �`��܂��͕ʃN���X�ɂ�������A�������肷�邩��
    TextProperty title_property_;
    Surface title_surface_;
    Component title_label_;
    Surface subTitle_surface_;
    Component subTitle_label_;
    size_t first_ticks_;

    PrologueState(CommonResources* common)
      : common_(common),
        title_property_(common_->font_, PracticeMenu_shimonoku, TitleSize,
                       Fore, Back, true),
        title_surface_(new TextSurface(title_property_)),
        title_label_(new LabelComponent(title_surface_)) {
    }

  public:
    static PrologueState* getObject(CommonResources* common) {
      static PrologueState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // �u���̋�^�C�s���O�v�ƕ\��
      SDL_Rect position;
      set_SdlRect(&position, centerPosition(title_label_, 640/2),
                  middlePosition(title_label_, 480/4 + SubOffset));
      title_label_->setPosition(&position);

      // �uX ��v�̃��x���쐬
      std::vector<Uint16> num_str;
      char buffer[4];
      sprintf(buffer, "%d", common_->getPracticeNum());
      ustrcat(num_str, buffer);
      ustrcat(num_str, ShimonokuPractice_PrologueSubTitle);
      TextProperty text_property(common_->font_, &num_str[0],
                                 LargeSize, Fore, Back, true);
      Surface new_surface(new TextSurface(text_property));
      std::swap(subTitle_surface_, new_surface);

      Component new_label(new LabelComponent(subTitle_surface_));
      std::swap(subTitle_label_, new_label);

      set_SdlRect(&position, centerPosition(subTitle_label_, 640/2),
                  topPosition(subTitle_label_, 480/4 + MenuOffset) + SubOffset);
      subTitle_label_->setPosition(&position);

      placeComponents();

      // ���y�̒�~��҂�
      // !!! �^�C���A�E�g�ł���悤�ɂ��ׂ�����
      while (common_->nowSoundPlaying()) {
        delay(10);
      }

      // ���ʉ��̍Đ��J�n
      common_->playEffect(SoundEffect_1);
      first_ticks_ = GetTicks();

      common_->gui_->update();
      // �a�� CD-ROM ���Z�b�g����Ă��邩�A�܂��͉����f�[�^�����邩�̊m�F
      // !!! �������X���b�h�ɂ��Ȃ��ƁA�`�悪�����A����
      common_->voice_->checkResource(common_->getVoiceResource());
    }

    void execute(Schedule* type) {

      // �ҋ@���Ԃ��o�߂�����ATypingState �֑J�ڂ���
      InputEvent input_event;
      common_->input_receiver_->updateInputEvent(input_event);
      size_t spent_msec = GetTicks() - first_ticks_;
      if ((spent_msec >= PrologueMsec) ||
          (InputEvent::isReleased(input_event, SDLK_RETURN))) {
        type->changeState(TypingState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      // �z�u�R���|�[�l���g���폜
      removeComponents();

      // ���y�̍Đ�
      common_->setNextMusic(BackMusic_2);
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(title_label_);
      common_->front_layer_->push_front(subTitle_label_);

      common_->input_receiver_->clear();
    }

    void removeComponents(void) {
      common_->front_layer_->remove(title_label_);
      common_->front_layer_->remove(subTitle_label_);

      common_->stopEffect();
    }
  };

  class TypingState : public State<Schedule> {
    CommonResources* common_;
    ShimonokuInput shimonoku_input_;
    InputEvent input_event_;
    size_t practice_left_;
    bool typing_complete_;

    TypingState(CommonResources* common)
      : common_(common),
        shimonoku_input_(ShimonokuTyping, common_->getConvertType()),
        typing_complete_(false) {
    }

  public:
    static TypingState* getObject(CommonResources* common) {
      static TypingState obj(common);
      return &obj;
    }

    void setPracticeNum(size_t num) {
      practice_left_ = num;
    }

    void removeComponents(void) {
      shimonoku_input_.removeComponents();
      shimonoku_input_.removeLeftNum();
      common_->voice_->stop();
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // �r�ݏグ�̊J�n
      common_->gui_->update();
      // !!! �������X���b�h�ɂ��Ȃ��ƁA�`�悪�����
      common_->voice_->play(shimonoku_input_.getCurrentWakaNo());

      typing_complete_ = false;
      shimonoku_input_.updateWaka();

      common_->input_receiver_->clear();
    }

    void execute(Schedule* type) {

      // ��̋�A���̋�A���͌���`��
      shimonoku_input_.drawWaka();

      // ���͊�����A���s���������̂�҂�
      common_->input_receiver_->updateInputEvent(input_event_);
      if (typing_complete_) {

        // ���s�̓��͂�҂�
        if (! InputEvent::isPressed(input_event_, SDLK_RETURN)) {
          return;
        }

        typing_complete_ = false;
        if (practice_left_ <= 0) {
          // ���K������������AResultState �֑J�ڂ���
          type->changeState(ResultState::getObject(common_));

          // �r�ݏグ�̒�~
          common_->voice_->stop();

        } else {
          // �ēx�A�a�̂̓��͂��s��
          shimonoku_input_.setNextWaka();
          shimonoku_input_.removeLeftNum();
          type->changeState(TypingState::getObject(common_));
        }
        return;
      }

      // ���̋�̓��͗�������
      if (! typing_complete_) {
        shimonoku_input_.drawInput();
      }

      // ���͂����������玟�̘a�̂�
      if (shimonoku_input_.isComplete()) {
        --practice_left_;
        shimonoku_input_.removeInputComponents();
        shimonoku_input_.placeLeftNum(practice_left_);
        typing_complete_ = true;

        // ���ʉ��̍Đ�
        common_->playEffect(Decide);
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      // �z�u�R���|�[�l���g���폜
      removeComponents();

      common_->voice_->stop();
    }

    bool textEmpty(void) {
      return shimonoku_input_.textEmpty();
    }
  };


  class ResultState : public State<Schedule> {
    CommonResources* common_;
    ResultDrawer result_drawer_;
    size_t first_ticks_;

    ResultState(CommonResources* common)
      : common_(common), result_drawer_(ShimonokuTyping) {
    }

  public:
    static ResultState* getObject(CommonResources* common) {
      static ResultState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // �^�C�s���O���ʂ̕]��
      // !!! ���ۂ́A�]���ƃT�[�o�ւ̃f�[�^���M�A�Ȃǂ��H

      // �\���p�^�C�}�[�J�E���^�̏�����
      result_drawer_.createResult();
      first_ticks_ = GetTicks();
    }

    void execute(Schedule* type) {

      // �^�C�s���O�̕]����\��
      size_t ticks = GetTicks() - first_ticks_;
      result_drawer_.drawResult(ticks);

      // �L�[���͂ȂǂŁA�O���t�\����
      if (result_drawer_.keyPressed()) {
        type->changeState(WaitKeyState::
                          getObject(common_, GraphState::getObject(common_)));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }

    void removeComponents(void) {
      result_drawer_.removeComponents();
    }
  };

  class GraphState : public State<Schedule> {
    CommonResources* common_;
    GraphDrawer graph_drawer_;

    GraphState(CommonResources* common)
      : common_(common), graph_drawer_(ShimonokuTyping) {
    }

  public:
    static GraphState* getObject(CommonResources* common) {
      static GraphState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // ���ʕ\���̍폜
      ResultState::getObject(common_)->removeComponents();

      // �O���t�̕`��
      if (graph_drawer_.drawGraph() == false) {
        // !!! ���̏�Ԃɕ��򂷂ׂ�
        // !!! �t�@�C���p�[�~�b�V�����܂��Ƃ��H
        return;
      }
      graph_drawer_.placeComponents();
    }

    void execute(Schedule* type) {

      // �L�[���͂ȂǂŁA�I��
      if (graph_drawer_.keyPressed()) {
        type->changeState(FirstState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }

    void removeComponents(void) {
      graph_drawer_.removeComponents();
    }
  };

  // !!! ���̃N���X�́A���̂Ƌ��ʂɂȂ�̂ŁA�����肽��
  class WaitKeyState : public State<Schedule> {
    CommonResources* common_;
    InputEvent input_event_;
    State<Schedule>* next_state_;

    WaitKeyState(CommonResources* common, State<Schedule>* next_state)
      : common_(common), next_state_(next_state) {
    }

  public:
    static WaitKeyState* getObject(CommonResources* common,
                                   State<Schedule>* next_state) {
      static WaitKeyState obj(common, next_state);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      common_->input_receiver_->clear();
    }

    void execute(Schedule* type) {
      common_->input_receiver_->updateInputEvent(input_event_);
      if (InputEvent::isReleased(input_event_, SDLK_RETURN)) {
        type->changeState(next_state_);
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void removeComponents(void) {
    }
  };

  CommonResources* common_;
  Schedule scheduler_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      scheduler_(0, common_) {
    TypingState::getObject(common_)->setPracticeNum(common_->getPracticeNum());

    // �x���ɑΏ�
    //if (PracticeMenu_title || PracticeMenu_kimariji || PracticeMenu_back);
  }

  void removeComponents(void) {
    PrologueState::getObject(common_)->removeComponents();
    TypingState::getObject(common_)->removeComponents();
    ResultState::getObject(common_)->removeComponents();
    GraphState::getObject(common_)->removeComponents();
    WaitKeyState::getObject(common_, NULL)->removeComponents();
  }
};


ShimonokuPractice::ShimonokuPractice(void) : pimpl(new pImpl) {
}


ShimonokuPractice::~ShimonokuPractice(void) {
}


void ShimonokuPractice::run(void) {

  GuiManager* gui = pimpl->common_->gui_;
  InputHandler& input = *pimpl->common_->input_;
  InputEvent input_event;
  bool quit = false;
  bool escape_pressed = false;
  while (quit == false) {

    // ��Ԃ̍X�V
    pimpl->scheduler_.update();

    // �I������
    input.updateInputEvent(input_event);
    quit |= pimpl->scheduler_.isTerminated() | input_event.right_clicked;

    input.update_all();
    gui->update();
    if (input.haveQuitEvent()) {
      pimpl->common_->front_layer_->disable();
      quit |= true;
    }
    if (pImpl::TypingState::getObject(pimpl->common_)->textEmpty()) {
      escape_pressed |= input.isPressed(SDLK_ESCAPE);
      if (escape_pressed && input.isReleased(SDLK_ESCAPE)) {
        quit |= true;
      }
    }
    delay(1);
  }

  // �e�R���|�[�l���g�̍폜
  pimpl->removeComponents();
}
