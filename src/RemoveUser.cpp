/*!
  \file
  \brief ユーザ削除

  \author Satofumi KAMIMURA

  $Id$
*/

#include "RemoveUser.h"
#include "RemoveUser_uni.h"
#include "CommonResources.h"
#include "SystemDefinition.h"
#include "DrawsDefinition.h"
#include "AccessSettings.h"
#include "arrow_bmp.h"
#include "warn_bmp.h"
#include "GuiManager.h"
#include "Layer.h"
#include "InputHandler.h"
#include "InputEvent.h"
#include "SdlSurface.h"
#include "SwitchSurface.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "ColorSurface.h"
#include "LabelComponent.h"
#include "SwitchLabelComponent.h"
#include "ButtonComponent.h"
#include "MenuComponent.h"
#include "SelectComponent.h"
#include "CreateSurfaceFromArray.h"
#include "BaseEntity.h"
#include "StateMachine.h"
#include "SdlUtils.h"
#include "Delay.h"
#include "UtfString.h"

using namespace beego;


struct RemoveUser::pImpl {

  enum { MessageY = 285 };

  class Schedule : public BaseEntity {
    StateMachine<Schedule>* state_machine_;
    bool is_terminated_;

  public:
    Schedule(int id, CommonResources* common_)
      : BaseEntity(id), is_terminated_(false) {
      state_machine_ = new StateMachine<Schedule>(this);
      state_machine_->setCurrentState(FirstState::getObject(common_));
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

    FirstState(CommonResources* common_) : common_(common_) {
    }

  public:
    static FirstState* getObject(CommonResources* common_) {
      static FirstState obj(common_);
      return &obj;
    }

    void enter(Schedule* type) {
      type->setTerminate();
    }

    void execute(Schedule* type) {
      type->changeState(SelectUserState::getObject(common_));
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }
  };

  class SelectUserState : public State<Schedule> {
    enum {
      RectWidth = MenuSize * 18/2,
    };
    CommonResources* common_;
    boost::shared_ptr<MenuComponent> menu_;
    Surface back_surface_;
    Component back_label_;
    Surface arrow_surface_;
    Component arrow_label_;
    boost::shared_ptr<SwitchSurface> message_;
    Component message_label_;

    SelectUserState(CommonResources* common_)
      : common_(common_), menu_(new MenuComponent),
        back_surface_(new ColorSurface(RectWidth,
                                       MenuSize * (UserMax + 1), Black)),
        back_label_(new LabelComponent(back_surface_)),
        arrow_surface_(new SdlSurface(createSurface(arrow_bmp,
                                                    arrow_bmp_width,
                                                    arrow_bmp_height), true)),
        arrow_label_(new LabelComponent(arrow_surface_)) {
    }

  public:
    static SelectUserState* getObject(CommonResources* common_) {
      static SelectUserState obj(common_);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // ユーザ名一覧メニューの作成
      int user_num = 0;
      size_t max_width = 0;
      menu_->clearItems();
      for (std::vector<std::string>::iterator it =
             common_->settings_->user_names_.begin();
           it != common_->settings_->user_names_.end(); ++it) {

        const char* user_name = it->c_str();
        if (strlen(user_name) <= 0) {
          continue;
        }
        ++user_num;

        // ユーザ項目の追加
        boost::shared_ptr<ButtonComponent> new_button(new ButtonComponent);
        TextProperty normal(common_->font_, user_name, MenuSize,
                            White, Gray3, true);
        TextProperty selected(common_->font_, user_name, MenuSize,
                              White, Gray3, true);
        ButtonComponent::createButton(new_button, normal, selected, selected);
        if (new_button->getWidth() > max_width) {
          max_width = new_button->getWidth();
        }
        menu_->addItem(new_button);
      }
      menu_->setItemsOffset(0, MenuSize);
      menu_->setItemSelectWidth(max_width);
      menu_->setIconComponent(arrow_label_, -24, 3, Middle | Left);
      menu_->releaseDecided();

      // 表示メッセージの作成
      boost::shared_ptr<SwitchSurface> new_message(new SwitchSurface);
      std::swap(message_, new_message);
      int index = 0;
      for (std::vector<std::string>::iterator it =
             common_->settings_->user_names_.begin();
           it != common_->settings_->user_names_.end(); ++it, ++index) {

        const char* user_name = it->c_str();
        if (strlen(user_name) <= 0) {
          continue;
        }

        if (index == 0) {
          // 「デフォルトユーザは削除できません」
          TextProperty no_remove(common_->font_, RemoveUser_no_remove,
                                 NormalSize, Fore, Back, true);
          Surface no_remove_surface(new TextSurface(no_remove));
          message_->registerSurface(no_remove_surface, 0, 0, index);

        } else {
          // 「"ユーザ名" を削除します」
          std::vector<Uint16> text;
          ustrcat(text, user_name);
          ustrcat(text, RemoveUser_remove);
          TextProperty text_property(common_->font_, &text[0],
                                     NormalSize, Fore, Back, true);
          Surface surface(new TextSurface(text_property));
          message_->registerSurface(surface, 0, 0, index);
        }
      }
      // メニューの初期設定の更新
      if (user_num > 1) {
        menu_->setSelected(1);
        message_->switchSurface(1);
      } else {
        // デフォルトユーザしか、いないとき
        menu_->setSelected(0);
        message_->switchSurface(0);
      }

      int mode = SwitchLabelComponent::Center;
      Component new_message_label(new SwitchLabelComponent(message_, mode));
      std::swap(message_label_, new_message_label);

      // 位置の指定
      SDL_Rect position;
      set_SdlRect(&position, centerPosition(menu_, 640/2), 480/5);
      menu_->setPosition(&position);

      set_SdlRect(&position, centerPosition(back_label_, 640/2), 480/5);
      back_label_->setPosition(&position);

      set_SdlRect(&position, centerPosition(message_label_, 640/2),
                  MessageY);
      message_label_->setPosition(&position);

      placeComponents();
    }

    void execute(Schedule* type) {
      // メッセージの表示切り替え
      message_->switchSurface(menu_->getSelected());

      int decided = menu_->getDecided();
      if (decided == 0) {
        // デフォルトユーザ。削除させない
        menu_->releaseDecided();
      } else if (decided > 0) {
        // 削除確認へ
        type->changeState(YesNoState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }

    void placeComponents(void) {

      common_->front_layer_->push_front(back_label_);
      common_->front_layer_->push_front(menu_);
      common_->front_layer_->push_front(message_label_);
    }

    void removeComponents(void) {

      common_->front_layer_->remove(back_label_);
      common_->front_layer_->remove(menu_);
      common_->front_layer_->remove(message_label_);
    }

    int getRemoveUserIndex(void) {
      return menu_->getDecided();
    }
  };

  class YesNoState : public State<Schedule> {
    CommonResources* common_;
    TextProperty yes_no_property_;
    Surface yes_no_surface_;
    Component yes_no_label_;
    Surface warn_surface_;
    Component warn_label_;
    TextProperty selector_property_;
    boost::shared_ptr<SelectComponent> selector_;

    YesNoState(CommonResources* common_)
      : common_(common_),
        yes_no_property_(common_->font_, RemoveUser_yes_no,
                         NormalSize, Fore, Back, true),
        yes_no_surface_(new TextSurface(yes_no_property_)),
        yes_no_label_(new LabelComponent(yes_no_surface_)),
        warn_surface_(new SdlSurface(createSurface(warn_bmp,
                                                   warn_bmp_width,
                                                   warn_bmp_height), true)),
        warn_label_(new LabelComponent(warn_surface_)),
        selector_property_(common_->font_, "", NormalSize, Fore, Back, true),
        selector_(new SelectComponent(selector_property_, White,
                                      SelectComponent::UnderBar)) {

      selector_->addItem("Yes");
      selector_->addItem("No");
      selector_->setItemsOffset(16);
      selector_->enableInput();
    }

  public:
    static YesNoState* getObject(CommonResources* common_) {
      static YesNoState obj(common_);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // 「本当によろしいですか？」の配置
      SDL_Rect position;
      set_SdlRect(&position, centerPosition(yes_no_label_, 640/2) + 10
                  + (32 + TextOffset)/2,
                  MessageY + NormalSize + TextOffset);
      yes_no_label_->setPosition(&position);

      // 警告アイコンの配置
      position.x -= 32 + TextOffset - 2;
      position.y -= (32 - NormalSize)/2 + 2;
      warn_label_->setPosition(&position);

      // 「Yes / No」の配置
      set_SdlRect(&position, centerPosition(selector_, 640/2),
                  position.y + NormalSize + TextOffset);
      selector_->setPosition(&position);
      selector_->releaseDecided();
      selector_->setSelected(1);

      placeComponents();
    }

    void execute(Schedule* type) {

      int decided = selector_->getDecided();
      if (decided >= 0) {
        if (decided == 0) {
          // Yes
          int remove_index =
            SelectUserState::getObject(common_)->getRemoveUserIndex();
          common_->deleteUser(remove_index);
        }
        type->changeState(SelectUserState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(yes_no_label_);
      common_->front_layer_->push_front(warn_label_);
      common_->front_layer_->push_front(selector_);
    }

    void removeComponents(void) {
      SelectUserState::getObject(common_)->removeComponents();
      common_->front_layer_->remove(yes_no_label_);
      common_->front_layer_->remove(warn_label_);
      common_->front_layer_->remove(selector_);
    }
  };

  CommonResources* common_;
  Schedule scheduler_;
  TextProperty title_property_;
  Surface title_surface_;
  Component title_label_;
  TextProperty back_property_;
  TextProperty back_focus_property_;
  boost::shared_ptr<ButtonComponent> back_button_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      scheduler_(0, common_),
      title_property_(common_->font_, RemoveUser_title,
                      MenuSize, Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      back_property_(common_->font_, RemoveUser_back_message,
                     NormalSize, Fore, Back, true),
      back_focus_property_(common_->font_, RemoveUser_back_message,
                           NormalSize, Blue, Back, true),
      back_button_(new ButtonComponent()) {

    ButtonComponent::createButton(back_button_, back_property_,
                                  back_focus_property_, back_focus_property_);

    // タイトル位置
    SDL_Rect position;
    set_SdlRect(&position, centerPosition(title_label_, 640/2),
                topPosition(title_label_, 0) + TextOffset * 3);
    title_label_->setPosition(&position);

    // 戻るためのメッセージ位置
    set_SdlRect(&position, rightPosition(back_button_, 640) - TextOffset,
                bottomPosition(back_button_, 480) - TextOffset);
    back_button_->setPosition(&position);
  }

  void placeComponents(void) {
    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(back_button_);
  }

  void removeComponents(void) {
    YesNoState::getObject(common_)->removeComponents();
    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(back_button_);
  }
};


RemoveUser::RemoveUser(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


RemoveUser::~RemoveUser(void) {
  pimpl->removeComponents();
}


void RemoveUser::run(void) {

  GuiManager* gui = pimpl->common_->gui_;
  InputHandler& input = *pimpl->common_->input_;
  InputEvent input_event;
  bool quit = false;
  bool escape_pressed = false;
  while (quit == false) {

    // 状態の更新
    pimpl->scheduler_.update();

    // 終了判定
    input.updateInputEvent(input_event);
    quit |= pimpl->scheduler_.isTerminated() ||
      pimpl->back_button_->isDecided() | input_event.right_clicked;
    gui->update();
    input.update_all();
    if (input.haveQuitEvent()) {
      pimpl->common_->front_layer_->disable();
      quit |= true;
    }
    escape_pressed |= input.isPressed(SDLK_ESCAPE);
    if (escape_pressed && input.isReleased(SDLK_ESCAPE)) {
      quit |= true;
    }
    delay(1);
  }
}
