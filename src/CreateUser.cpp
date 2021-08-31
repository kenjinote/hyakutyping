/*!
  \file
  \brief ユーザ作成

  \author Satofumi KAMIMURA

  $Id$

  \todo 認証画像が、読めない
  \todo PC 内のユーザ名と重複がないか、も判別する
  \todo TextInput よりも長いアドレスも入力可能にする
*/

#include "CreateUser.h"
#include "CreateUser_uni.h"
#include "CommonResources.h"
#include "SystemDefinition.h"
#include "DrawsDefinition.h"
#include "ResourceDefinition.h"
#include "AccessSettings.h"
#include "GuiManager.h"
#include "Layer.h"
#include "InputEvent.h"
#include "InputHandler.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "ImageSurface.h"
#include "LabelComponent.h"
#include "ButtonComponent.h"
#include "TextInputComponent.h"
#include "InputReceiveComponent.h"
#include "SelectComponent.h"
#include "BaseEntity.h"
#include "StateMachine.h"
#include "GetAccountName.h"
#include "RemoveFile.h"
#include "ThreadCreator.h"
#include "SdlUtils.h"
#include "Delay.h"
#include "GetTicks.h"
#include "UtfString.h"
#include "HttpAccess.h"
#include <fstream>

using namespace beego;


struct CreateUser::pImpl {
  enum {
    MessageY = 480 - (NormalSize * 4) - (TextOffset * 2),
    WarnY = MessageY - NormalSize * 4,

    HttpX = 20,
    HttpY = 100,
    HttpWidth = 510,

    NameWidth = 160,
    NameX = 20,
    NameY = 150,

    PasswordWidth = 100,
    PasswordX = 20,
    PasswordY = 200,

    GetAntiSpamImage,
    CommitPassword,
    None,

    AddressError = -1,
    UserExist = -2,
    PasswordMismatch = -3,
    UnknownError = -4,
  };

  class ThreadArgs {
    ThreadArgs(void);

  public:
    int type_;
    SDL_sem* result_;

    ThreadArgs(int type) : type_(type), result_(SDL_CreateSemaphore(0)) {
    }

    ~ThreadArgs(void) {
      SDL_DestroySemaphore(result_);
    }
  };

  class ProgressChanger {
    CommonResources* common_;
    TextProperty message_property_;
    Surface message_surface_;
    Component message_label_;
    boost::shared_ptr<SelectComponent> selector_;
    size_t now_progress_;
    Surface server_surface_;
    Component server_label_;
    Surface name_surface_;
    Component name_label_;

    ProgressChanger(CommonResources* common)
      : common_(common),
        message_property_(common_->font_, "Step:",
                          NormalSize, Fore, Back, true),
        message_surface_(new TextSurface(message_property_)),
        message_label_(new LabelComponent(message_surface_)),
        selector_(new SelectComponent(message_property_, White)),
        now_progress_(0) {

      // セレクターの初期化
      selector_->addItem("1");
      selector_->addItem("2");
      selector_->addItem("3");
      //selector_->addItem("4");
      selector_->setItemsOffset(NormalSize/2);
      selector_->disableInput();

      SDL_Rect position;
      set_SdlRect(&position, TextOffset,
                  bottomPosition(selector_, MessageY) - TextOffset);
      message_label_->setPosition(&position);

      set_SdlRect(&position, position.x +
                  static_cast<int>(message_label_->getWidth()) + TextOffset,
                  bottomPosition(selector_, MessageY) - TextOffset);
      selector_->setPosition(&position);
    }

    void updateSurfaces(void) {

      removeComponents();

      // セレクタ位置の調整
      if (now_progress_ >= 3) {
        selector_->setSelected(static_cast<int>(SelectComponent::NotSelected));
      } else {
        selector_->setSelected(static_cast<int>(now_progress_));
      }

      SDL_Rect position;
      if (now_progress_ > 0) {
        // サーバアドレス文字列の生成
        std::string http_text = "http://" +
          std::string(HttpInputState::getObject(common_)->getServerAddress());
        message_property_.text = http_text.c_str();
        Surface new_server_surface(new TextSurface(message_property_));
        std::swap(server_surface_, new_server_surface);

        Component new_server_label(new LabelComponent(server_surface_));
        std::swap(server_label_, new_server_label);

        set_SdlRect(&position, HttpX, HttpY);
        server_label_->setPosition(&position);
      }
      if (now_progress_ > 1) {
        // ログイン文字列の生成
        std::string name_text = "name: "
          + std::string(NameInputState::getObject(common_)->getLoginName());
        message_property_.text = name_text.c_str();
        Surface new_name_surface(new TextSurface(message_property_));
        std::swap(name_surface_, new_name_surface);

        Component new_name_label(new LabelComponent(name_surface_));
        std::swap(name_label_, new_name_label);

        set_SdlRect(&position, NameX, NameY);
        name_label_->setPosition(&position);
      }

      placeComponents();
    }

  public:
    static ProgressChanger* getObject(CommonResources* common) {
      static ProgressChanger obj(common);
      return &obj;
    }

    void changeProgress(int progress) {
      now_progress_ = progress;
      updateSurfaces();
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(message_label_);
      common_->front_layer_->push_front(selector_);

      if (now_progress_ > 0) {
        common_->front_layer_->push_front(server_label_);
      }
      if (now_progress_ > 1) {
        common_->front_layer_->push_front(name_label_);
      }
    }

    void removeComponents(void) {
      common_->front_layer_->remove(message_label_);
      common_->front_layer_->remove(selector_);

      if (now_progress_ > 0) {
        common_->front_layer_->remove(server_label_);
      }
      if (now_progress_ > 1) {
        common_->front_layer_->remove(name_label_);
      }
    }
  };

  class Schedule : public BaseEntity {
    StateMachine<Schedule>* state_machine_;
    bool is_terminated_;

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

  // 最初の状態
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
      type->changeState(HttpInputState::getObject(common_));
    }

    void exit(Schedule* type) {
      static_cast<void>(type);
    }
  };

  // サーバのアドレスを入力
  class HttpInputState : public State<Schedule> {
    CommonResources* common_;
    TextProperty text_property_;
    Surface http_surface_;
    Component http_label_;
    TextProperty message_property_;
    Surface http_message_surface_;
    Component http_message_label_;
    TextProperty input_property_;
    boost::shared_ptr<TextInputComponent> text_input_;
    std::vector<Uint16> default_address_;

    HttpInputState(CommonResources* common)
      : common_(common),
        text_property_(common_->font_, "http://", NormalSize, Fore, Back, true),
        http_surface_(new TextSurface(text_property_)),
        http_label_(new LabelComponent(http_surface_)),
        message_property_(common_->font_, CreateUser_http_message,
                          NormalSize, Fore, Back, true),
        http_message_surface_(new TextSurface(message_property_)),
        http_message_label_(new LabelComponent(http_message_surface_)),
        input_property_(common_->font_, "", NormalSize-3, White, Black, false),
        text_input_(new TextInputComponent(HttpWidth, NormalSize + TextOffset/4,
                                           input_property_, TextOffset/2)) {

      // コンポーネントの位置設定
      SDL_Rect position;
      set_SdlRect(&position, HttpX, HttpY);
      http_label_->setPosition(&position);

      set_SdlRect(&position,
                  static_cast<int>(HttpX + http_label_->getWidth()), HttpY);
      text_input_->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(http_message_label_, 640/2), MessageY);
      http_message_label_->setPosition(&position);

      // !!! ま、とりあえずデフォルトは固定にしてしまう
      // !!! 余力あらば、ユーザが最後に入力した有効なアドレスを保持する、とか
      // !!! 直前のユーザのと同じにするとか
      for (int i = 0; DefaultServerAddress[i] != 0x0; ++i) {
        default_address_.push_back(DefaultServerAddress[i]);
      }
    }

  public:
    static HttpInputState* getObject(CommonResources* common) {
      static HttpInputState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // コンポーネントの初期化
      text_input_->releaseDecided();
      text_input_->setBuffer(default_address_);

      ProgressChanger::getObject(common_)->changeProgress(0);

      placeComponents();
    }

    void execute(Schedule* type) {

      if (text_input_->isDecided()) {

        // 入力アドレスが有効かを確認し、有効でなければメッセージを表示する
        // !!! 確認方法を、おいおい考える

        // ユーザ名の入力へ分岐
        type->changeState(NameInputState::getObject(common_));
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {

      common_->front_layer_->push_front(http_label_);
      common_->front_layer_->push_front(text_input_);
      common_->front_layer_->push_front(http_message_label_);
    }

    void removeComponents(void) {
      common_->front_layer_->remove(http_label_);
      common_->front_layer_->remove(text_input_);
      common_->front_layer_->remove(http_message_label_);
    }

    const char* getServerAddress(void) {

      static std::string address_str;
      std::vector<Uint16> input_buffer;

      // 利用していないはずなので、Lock しない
      text_input_->getBuffer(input_buffer);

      std::vector<Uint16> address;
      ustrcat(address, &input_buffer[0]);
      address.pop_back();

      address_str.clear();
      for (std::vector<Uint16>::iterator it = address.begin();
           it != address.end(); ++it) {
        char ch = *it & 0xff;
        if (isprint(ch)) {
          address_str.push_back(ch);
        }
      }
      return address_str.c_str();
    }
  };

  // ユーザ名を入力
  class NameInputState : public State<Schedule> {
    CommonResources* common_;
    ThreadArgs thread_args_;
    ThreadCreator thread_;

    TextProperty text_property_;
    Surface login_surface_;
    Component login_label_;
    TextProperty message_property_;
    Surface message_surface_;
    Component message_label_;
    TextProperty input_property_;
    boost::shared_ptr<TextInputComponent> text_input_;
    InputEvent input_event_;
    bool web_accessed_;
    bool aborted_;
    bool is_pressed_;
    size_t first_ticks_;

    TextProperty now_request_property_;
    Surface now_request_surface_;
    Component now_request_label_;
    TextProperty no_reply_property_;
    Surface no_reply_surface_;
    Component no_reply_label_;
    TextProperty user_exist_property_;
    Surface user_exist_surface_;
    Component user_exist_label_;
    TextProperty address_error_property_;
    Surface address_error_surface_;
    Component address_error_label_;

    NameInputState(CommonResources* common)
      : common_(common),
        thread_args_(GetAntiSpamImage),
        thread_(webAccess_thread, &thread_args_),
        text_property_(common_->font_, "name:", NormalSize, Fore, Back, true),
        login_surface_(new TextSurface(text_property_)),
        login_label_(new LabelComponent(login_surface_)),
        message_property_(common_->font_, CreateUser_name_message,
                          NormalSize, Fore, Back, true),
        message_surface_(new TextSurface(message_property_)),
        message_label_(new LabelComponent(message_surface_)),
        input_property_(common_->font_, "", NormalSize, White, Black, false),
        text_input_(new TextInputComponent(NameWidth, NormalSize + TextOffset/4,
                                           input_property_, TextOffset/2)),
        web_accessed_(false), aborted_(false), is_pressed_(false),
        now_request_property_(common_->font_, CreateUser_now_request,
                              NormalSize, Fore, Back, true),
        now_request_surface_(new TextSurface(now_request_property_)),
        now_request_label_(new LabelComponent(now_request_surface_)),
        no_reply_property_(common_->font_, CreateUser_no_reply,
                           NormalSize, Fore, Back, true),
        no_reply_surface_(new TextSurface(no_reply_property_)),
        no_reply_label_(new LabelComponent(no_reply_surface_)),
        user_exist_property_(common_->font_, CreateUser_user_exist,
                             NormalSize, Fore, Back, true),
        user_exist_surface_(new TextSurface(user_exist_property_)),
        user_exist_label_(new LabelComponent(user_exist_surface_)),
        address_error_property_(common_->font_, CreateUser_address_error,
                                NormalSize, Fore, Back, true),
        address_error_surface_(new TextSurface(address_error_property_)),
        address_error_label_(new LabelComponent(address_error_surface_)) {

      // コンポーネントの位置設定
      SDL_Rect position;
      set_SdlRect(&position, NameX, NameY);
      login_label_->setPosition(&position);

      set_SdlRect(&position, NameX + static_cast<int>(login_label_->getWidth())
                  + TextOffset, NameY);
      text_input_->setPosition(&position);

      set_SdlRect(&position, centerPosition(message_label_, 640/2), MessageY);
      message_label_->setPosition(&position);

      // 中断メッセージの位置設定
      set_SdlRect(&position, centerPosition(now_request_label_, 640/2), WarnY);
      now_request_label_->setPosition(&position);

      set_SdlRect(&position, centerPosition(no_reply_label_, 640/2), WarnY);
      no_reply_label_->setPosition(&position);

      set_SdlRect(&position, centerPosition(user_exist_label_, 640/2), WarnY);
      user_exist_label_->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(address_error_label_, 640/2), WarnY);
      address_error_label_->setPosition(&position);
    }

  public:
    static NameInputState* getObject(CommonResources* common) {
      static NameInputState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      text_input_->releaseDecided();

      // デフォルト名を格納してしまう
      // !!! Windows などで、マルチバイト文字列の場合は、不明
      // !!! そもそも、上記の場合に GetUserName() が何を返すか、すら不明
      std::vector<Uint16> default_name;
      ustrcat(default_name, GetAccountName().c_str());

      // GetUserName が付加する末尾の '\0' を取り除く
      default_name.pop_back();
      text_input_->setBuffer(default_name);

      ProgressChanger::getObject(common_)->changeProgress(1);

      placeComponents();

      // Web アクセス状態の初期化
      web_accessed_ = false;
      aborted_ = false;
      SDL_DestroySemaphore(thread_args_.result_);
      thread_args_.result_ = SDL_CreateSemaphore(0);
      is_pressed_ = false;
    }

    void execute(Schedule* type) {

      if (text_input_->isDecided()) {
        if (aborted_) {
          // エラーのため処理を中止。改行か、ESC で抜けるのを待つ
          common_->input_receiver_->updateInputEvent(input_event_);
          if (! is_pressed_) {
            if (InputEvent::isPressed(input_event_, SDLK_RETURN)) {
              is_pressed_ = true;
            }
          } else {
            if (InputEvent::isReleased(input_event_, SDLK_RETURN)) {
              type->changeState(FirstState::getObject(common_));
            }
          }
          return;
        }

        // 確認中のメッセージを表示し、ユーザ作成ページへアクセスする
        if (! web_accessed_) {
          web_accessed_ = true;
          thread_.run(1);
          first_ticks_ = GetTicks();

          // 「サーバに問い合わせ中です」メッセージの配置
          common_->front_layer_->push_front(now_request_label_);
        }

        if (! thread_.isRunning()) {
          int ret = 0;
          thread_.wait(&ret);
          if (SDL_SemValue(thread_args_.result_) > 0) {
            // 画像が取得できてたら、ユーザ未登録。次の処理へ
            type->changeState(PasswordInputState::getObject(common_));

          } else {
            // 画像が取得できなかったら、メッセージを表示して終了
            // 現在の表示メッセージの削除
            // !!! removeComponents() でもよいかも
            common_->front_layer_->remove(now_request_label_);
            if (ret == AddressError) {
              // 「登録ページのアドレスが異なります」
              common_->front_layer_->push_front(address_error_label_);

            } else if (ret == UserExist) {
              // 「そのユーザ名は、既に使われています」
              common_->front_layer_->push_front(user_exist_label_);
            }
            common_->input_receiver_->clear();
            aborted_ = true;
          }
        } else if ((GetTicks() - first_ticks_) > WebAccessTimeout) {
          // 接続タイムアウト。メッセージを表示して終了
          common_->front_layer_->remove(now_request_label_);

          // 「サーバが応答しませんでした。処理を終了します」
          common_->front_layer_->push_front(no_reply_label_);
          common_->input_receiver_->clear();
          aborted_ = true;
        }
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      thread_.stop();
      removeComponents();
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(login_label_);
      common_->front_layer_->push_front(text_input_);
      common_->front_layer_->push_front(message_label_);
      common_->front_layer_->push_front(common_->input_receiver_);
    }

    void removeComponents(void) {
      common_->front_layer_->remove(login_label_);
      common_->front_layer_->remove(text_input_);
      common_->front_layer_->remove(message_label_);
      common_->front_layer_->remove(common_->input_receiver_);
      common_->front_layer_->remove(now_request_label_);
      common_->front_layer_->remove(no_reply_label_);
      common_->front_layer_->remove(user_exist_label_);
      common_->front_layer_->remove(address_error_label_);

      thread_.stop();
    }

    const char* getLoginName(void) {

      static std::vector<char> user_name;

      std::vector<Uint16> utext;
      user_name.clear();
      text_input_->getBuffer(utext);
      uni2char(user_name, utext);

      return &user_name[0];
    }

    int saveAntispamImage(void) {

      // サーバ名の取得
      std::string server_address =
        HttpInputState::getObject(common_)->getServerAddress();

      // ユーザ名の取得
      const char* user_name = getLoginName();

      // HTTP 要求文字列の生成
      size_t first_slash = server_address.find('/');
      std::string server;
      std::string address;
      if (first_slash != std::string::npos) {
        server = server_address.substr(0, first_slash);
        address = server_address.substr(first_slash, std::string::npos)
          + "registerUser.php?name=" + std::string(&user_name[0]);
      } else {
        server = server_address;
        // !!! 重複をなんとかする
        address = "/registerUser.php?name=" + std::string(&user_name[0]);
      }

      // HTTP アクセスを行い、認証画像を取得する
      HttpAccess http_access(server.c_str());
      std::vector<char> image_data;
      http_access.get(image_data, address.c_str());
      if (image_data.empty()) {
        // ユーザが既に存在する
        return UserExist;
      } else if (image_data[0] == '<') {
        // アドレス間違い
        return AddressError;
      }
      std::string save_path = common_->savedir_ + "antispam.jpg";
      std::ofstream image_file(save_path.c_str(), std::ios::binary);
      image_file.write(&image_data[0], static_cast<int>(image_data.size()));

      return 0;
    }
  };

  // !!! さすがに、重複が多い気がしてきた。何かのタイミングで関数化する
  // 認証用の文字列入力
  class PasswordInputState : public State<Schedule> {
    CommonResources* common_;

    class ThreadArgs {
      ThreadArgs(void);

    public:
      int type_;
      SDL_sem* result_;

      ThreadArgs(int type) : type_(type), result_(SDL_CreateSemaphore(0)) {
      }

      ~ThreadArgs(void) {
        SDL_DestroySemaphore(result_);
      }
    };
    ThreadArgs thread_args_;
    ThreadCreator thread_;
    InputEvent input_event_;
    bool web_accessed_;
    bool aborted_;
    bool is_pressed_;
    size_t first_ticks_;

    Surface image_surface_;
    Component image_label_;
    TextProperty message_property_;
    Surface message_surface_;
    Component message_label_;
    TextProperty input_property_;
    boost::shared_ptr<TextInputComponent> text_input_;
    TextProperty invalid_password_property_;
    Surface invalid_password_surface_;
    Component invalid_password_label_;
    // !!! 以下は、共用にすべき
    TextProperty now_request_property_;
    Surface now_request_surface_;
    Component now_request_label_;
    TextProperty no_reply_property_;
    Surface no_reply_surface_;
    Component no_reply_label_;
    TextProperty address_error_property_;
    Surface address_error_surface_;
    Component address_error_label_;
    PasswordInputState(CommonResources* common)
      : common_(common),
        thread_args_(CommitPassword),
        thread_(webAccess_thread, &thread_args_),
        web_accessed_(false), aborted_(false), is_pressed_(false),
        // !!! ファイル名を ResourceDefinition に登録する
        // !!! まぁ、実際には Web から取得する訳だが...
        message_property_(common_->font_, CreateUser_password_message,
                          NormalSize, Fore, Back, true),
        message_surface_(new TextSurface(message_property_)),
        message_label_(new LabelComponent(message_surface_)),
        input_property_(common_->font_, "", NormalSize, White, Black, false),
        text_input_(new TextInputComponent(PasswordWidth,
                                           NormalSize + TextOffset/4,
                                           input_property_, TextOffset/2)),
        invalid_password_property_(common_->font_, CreateUser_invalid_password,
                                   NormalSize, Fore, Back, true),
        invalid_password_surface_(new TextSurface(invalid_password_property_)),
        invalid_password_label_(new LabelComponent(invalid_password_surface_)),
        now_request_property_(common_->font_, CreateUser_now_request,
                              NormalSize, Fore, Back, true),
        now_request_surface_(new TextSurface(now_request_property_)),
        now_request_label_(new LabelComponent(now_request_surface_)),
        no_reply_property_(common_->font_, CreateUser_no_reply,
                           NormalSize, Fore, Back, true),
        no_reply_surface_(new TextSurface(no_reply_property_)),
        no_reply_label_(new LabelComponent(no_reply_surface_)),
        address_error_property_(common_->font_, CreateUser_address_error,
                                NormalSize, Fore, Back, true),
        address_error_surface_(new TextSurface(address_error_property_)),
        address_error_label_(new LabelComponent(address_error_surface_)) {
    }

  public:
    static PasswordInputState* getObject(CommonResources* common) {
      static PasswordInputState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      // 画像の更新
      std::string save_path = common_->savedir_ + "antispam.jpg";
      Surface new_image_surface(new ImageSurface(save_path.c_str()));
      std::swap(image_surface_, new_image_surface);
      Component new_image_label(new LabelComponent(image_surface_));
      std::swap(image_label_, new_image_label);
      removeFile(save_path.c_str());

      SDL_Rect position;
      set_SdlRect(&position, PasswordX, PasswordY);
      image_label_->setPosition(&position);

      // コンポーネントの位置設定
      set_SdlRect(&position, PasswordX +
                  static_cast<int>(image_label_->getWidth()) + TextOffset,
                  middlePosition(text_input_, PasswordY +
                                 static_cast<int>
                                 (image_label_->getHeight()/2)));
      text_input_->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(message_label_, 640/2), MessageY);
      message_label_->setPosition(&position);

      // 中断メッセージの位置設定
      set_SdlRect(&position, centerPosition(now_request_label_, 640/2), WarnY);
      now_request_label_->setPosition(&position);

      set_SdlRect(&position, centerPosition(no_reply_label_, 640/2), WarnY);
      no_reply_label_->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(invalid_password_label_, 640/2), WarnY);
      invalid_password_label_->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(address_error_label_, 640/2), WarnY);
      address_error_label_->setPosition(&position);

      // コンポーネントの配置
      text_input_->releaseDecided();
      std::vector<Uint16> null_string;
      text_input_->setBuffer(null_string);

      ProgressChanger::getObject(common_)->changeProgress(2);

      placeComponents();

      // Web アクセス状態の初期化
      SDL_DestroySemaphore(thread_args_.result_);
      thread_args_.result_ = SDL_CreateSemaphore(0);
      web_accessed_ = false;
      aborted_ = false;
      is_pressed_ = false;
    }

    void execute(Schedule* type) {

      if (text_input_->isDecided()) {
        if (aborted_) {
          // エラーのため処理を中止。改行か、ESC で抜けるのを待つ
          common_->input_receiver_->updateInputEvent(input_event_);
          if (! is_pressed_) {
            if (InputEvent::isPressed(input_event_, SDLK_RETURN)) {
              is_pressed_ = true;
            }
          } else {
            if (InputEvent::isReleased(input_event_, SDLK_RETURN)) {
              type->changeState(FirstState::getObject(common_));
            }
          }
          return;
        }

        // 確認中のメッセージを表示し、パスワード登録ページへアクセスする
        if (! web_accessed_) {
          web_accessed_ = true;
          thread_.run(1);
          first_ticks_ = GetTicks();

          // 「サーバに問い合わせ中です」メッセージの配置
          common_->front_layer_->push_front(now_request_label_);
        }

        if (! thread_.isRunning()) {
          int ret = 0;
          thread_.wait(&ret);
          if (SDL_SemValue(thread_args_.result_) > 0) {
            // 取得文字列が "O.K." だったら終了

            // ユーザの追加
            const char* user_name =
              NameInputState::getObject(common_)->getLoginName();
            if (strlen(user_name) > 0) {
              const char* password = getPassword();
              common_->addUser(user_name, password,
                               HttpInputState::getObject(common_)
                               ->getServerAddress());
              common_->settings_->save();
            }
            type->changeState(CompleteState::getObject(common_));

          } else {
            // 現在の登録メッセージを削除
            common_->front_layer_->remove(now_request_label_);
            if (ret == AddressError) {
              // 「登録ページのアドレスが異なります」
              common_->front_layer_->push_front(address_error_label_);

            } else if (ret == PasswordMismatch) {
              // 「パスワードが違います」
              common_->front_layer_->push_front(invalid_password_label_);
            }
            common_->input_receiver_->clear();
            aborted_ = true;
          }
        } else if ((GetTicks() - first_ticks_) > WebAccessTimeout) {
          // 接続タイムアウト。メッセージを表示して終了
          common_->front_layer_->remove(now_request_label_);

          // 「サーバが応答しませんでした。処理を終了します」
          common_->front_layer_->push_front(no_reply_label_);
          common_->input_receiver_->clear();
          aborted_ = true;
        }
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(text_input_);
      common_->front_layer_->push_front(image_label_);
      common_->front_layer_->push_front(message_label_);
    }

    void removeComponents(void) {
      common_->front_layer_->remove(text_input_);
      common_->front_layer_->remove(image_label_);
      common_->front_layer_->remove(message_label_);
      common_->front_layer_->remove(now_request_label_);
      common_->front_layer_->remove(no_reply_label_);
      common_->front_layer_->remove(invalid_password_label_);
      common_->front_layer_->remove(address_error_label_);
    }

    const char* getPassword(void) {
      static std::vector<char> password;

      std::vector<Uint16> utext;
      text_input_->getBuffer(utext);
      password.clear();
      uni2char(password, utext);

      return &password[0];
    }

    int commitPassword(void) {

      // !!! つか、saveAntispamImage() と同じ。くくる

      // サーバ名の取得
      std::string server_address =
        HttpInputState::getObject(common_)->getServerAddress();

      // ユーザ名の取得
      const char* user_name =
        NameInputState::getObject(common_)->getLoginName();

      // 入力されたパスワードの取得
      const char* inputed_password = getPassword();

      // HTTP 要求文字列の生成
      size_t first_slash = server_address.find('/');
      std::string server;
      std::string address;
      if (first_slash != std::string::npos) {
        server = server_address.substr(0, first_slash);
        address = server_address.substr(first_slash, std::string::npos)
          + "commitPassword.php?name=" + std::string(&user_name[0])
          + "&password=" + std::string(inputed_password);
      } else {
        server = server_address;
        // !!! 重複をなんとかする
        address = "/commitPassword.php?name=" + std::string(&user_name[0])
          + "&password=" + std::string(inputed_password);
      }

      // HTTP アクセスを行い、認証画像を取得する
      HttpAccess http_access(server.c_str());
      std::vector<char> http_reply;
      http_access.get(http_reply, address.c_str());
      if (http_reply.empty()) {
        // 既にユーザが存在する (複数からの同一ユーザ名の作成要求で起こりうる)
        return UserExist;

      } else if (http_reply[0] == '<') {
        // アドレス間違い
        return AddressError;

      } else if ((http_reply.size() != 4) ||
                 (strncmp(&http_reply[0], "O.K.", 4))) {
        return PasswordMismatch;
      }

      // !!! PC へのユーザの登録処理
      return 0;
    }
  };

  // 終了メッセージの表示
  class CompleteState : public State<Schedule> {
    CommonResources* common_;
    TextProperty message_property_;
    Surface message_surface_;
    Component message_label_;
    InputEvent input_event_;
    bool is_pressed_;

    CompleteState(CommonResources* common)
      : common_(common),
        message_property_(common_->font_, CreateUser_complete_message,
                          NormalSize, Fore, Back, true),
        message_surface_(new TextSurface(message_property_)),
        message_label_(new LabelComponent(message_surface_)),
        is_pressed_(false) {

      // コンポーネントの位置設定
      SDL_Rect position;
      //set_SdlRect(&position, rightPosition(name_label, NameX), NameY);
      //name_label->setPosition(&position);

      set_SdlRect(&position,
                  centerPosition(message_label_, 640/2), MessageY);
      message_label_->setPosition(&position);
    }

  public:
    static CompleteState* getObject(CommonResources* common) {
      static CompleteState obj(common);
      return &obj;
    }

    void enter(Schedule* type) {
      static_cast<void>(type);

      is_pressed_ = false;

      common_->input_receiver_->clear();
      ProgressChanger::getObject(common_)->changeProgress(3);

      placeComponents();
    }

    void execute(Schedule* type) {

      // 改行が押されたら、抜ける
      // !!! 離されたら、にしてみる
      common_->input_receiver_->updateInputEvent(input_event_);
      if (! is_pressed_) {
        if (InputEvent::isPressed(input_event_, SDLK_RETURN)) {
          is_pressed_ = true;
        }
      } else {
        if (InputEvent::isReleased(input_event_, SDLK_RETURN)) {
          type->changeState(FirstState::getObject(common_));
        }
      }
    }

    void exit(Schedule* type) {
      static_cast<void>(type);

      removeComponents();
    }

    void placeComponents(void) {
      common_->front_layer_->push_front(common_->input_receiver_);
      common_->front_layer_->push_front(message_label_);
    }

    void removeComponents(void) {
      common_->front_layer_->remove(common_->input_receiver_);
      common_->front_layer_->remove(message_label_);
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
  Surface back_surface_;

  pImpl(void)
    : common_(CommonResources::getObject()),
      scheduler_(0, common_),
      title_property_(common_->font_, CreateUser_title,
                      MenuSize, Fore, Back, true),
      title_surface_(new TextSurface(title_property_)),
      title_label_(new LabelComponent(title_surface_)),
      back_property_(common_->font_, CreateUser_back_message,
                     NormalSize, Fore, Back, true),
      back_focus_property_(common_->font_, CreateUser_back_message,
                           NormalSize, Blue, Back, true),
      back_button_(new ButtonComponent),
      back_surface_(new TextSurface(back_property_)) {

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

    // 警告に対処
    //if (CreateUser_name);
  }

  void placeComponents(void) {

    // 内部で配置もコンポーネントの行っている
    ProgressChanger::getObject(common_)->changeProgress(0);

    common_->front_layer_->push_front(title_label_);
    common_->front_layer_->push_front(back_button_);
  }

  void removeComponents(void) {
    ProgressChanger::getObject(common_)->removeComponents();
    HttpInputState::getObject(common_)->removeComponents();
    NameInputState::getObject(common_)->removeComponents();
    PasswordInputState::getObject(common_)->removeComponents();
    CompleteState::getObject(common_)->removeComponents();

    common_->front_layer_->remove(title_label_);
    common_->front_layer_->remove(back_button_);
  }

  static int webAccess_thread(void* args) {
    ThreadArgs* obj = static_cast<ThreadArgs*>(args);

    int ret = false;
    CommonResources* common = CommonResources::getObject();
    if (obj->type_ == GetAntiSpamImage) {
      // アンチスパム画像の取得
      ret = NameInputState::getObject(common)->saveAntispamImage();
      if (ret == 0) {
        SDL_SemPost(obj->result_);
      }
    } else if (obj->type_ == CommitPassword) {
      // パスワードの登録
      ret = PasswordInputState::getObject(common)->commitPassword();
      if (ret == 0) {
        SDL_SemPost(obj->result_);
      }
    }
    return ret;
  }
};


CreateUser::CreateUser(void) : pimpl(new pImpl) {
  pimpl->placeComponents();
}


CreateUser::~CreateUser(void) {
  pimpl->removeComponents();
}


void CreateUser::run(void) {

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
    quit |= pimpl->scheduler_.isTerminated() |
      pimpl->back_button_->isDecided() | input_event.right_clicked;
    input.update_all();
    gui->update();
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
