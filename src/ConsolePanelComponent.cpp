/*!
  \file
  \brief コンソール・コンポーネント

  \author Satofumi KAMIMURA

  $Id$

  \todo SDL 版でフェード処理が動作しないのを修正する。OpenGL 版では動作する
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

    // !!! 文字毎に position_changed を管理した方が、再描画が少ない

  } x_info_t;

  typedef struct {
    int y;
    std::deque<x_info_t> x_info;
    size_t surfaces_size;

    // !!! あと、行毎でも position_changed を管理すべきかも

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

  // !!! fore, back の _color を property 情報から受け取るべき
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

    // 最初の１行を追加
    updateToFirstState();
  }

  ~pImpl(void) {
    size_t n = draw_surfaces.size();
    for (size_t i = 0; i < n; ++i) {
      delete draw_surfaces.at(i);
    }
  }

  // １行分のサーフェスを削除
  void removeLine(void) {

    pImpl::y_info_t& line = surfaces.front();
    size_t remove_size = line.surfaces_size;
    for (size_t i = 0; i < remove_size; ++i) {
        draw_surfaces.rotate();
    }

    surfaces.pop_front();
    surfaces_size -= remove_size;
  }

  // 行クリア後の行情報を初期化
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

  // サーフェスを配置
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

  // 描画サーフェス再描画のための、全消去
  void eraseAllSurfaces(void) {
    //fprintf(stderr, "erase all surfaces\n");
    SurfaceList remove_list;
    size_t n = draw_surfaces.size();
    for (size_t i = 0; i < n; ++i) {
      // !!! at() よりも適切なメソッドに置き換える
      remove_list.push_back(draw_surfaces.at(i));
    }
    layer->removeSurfaces(remove_list);
  }

  // １行目の削除と位置の再評価を行う
  void scrollOut(void) {
    if (surfaces.empty()) {
      return;
    }

    //fprintf(stderr, "call scrollOut(): before %d\n", surfaces_size);

    // y の位置をずらす
    std::deque<y_info_t>::reverse_iterator it = surfaces.rbegin();
    for (std::deque<y_info_t>::reverse_iterator next_it = it + 1;
         next_it != surfaces.rend(); ++next_it, ++it) {
      it->y = next_it->y;
    }

    // 描画サーフェス再描画のための、全消去
    eraseAllSurfaces();

    // １行目の削除
    // 本来はここで資源を解放してはならないが、PointerRingBuffer なので O.K.
    removeLine();
  }

  void fadeOut(void) {
    if (surfaces.empty()) {
      return;
    }

    // y の位置を更新
    int y_offset = (direction == Down) ? +font_size : -font_size;
    for (std::deque<y_info_t>::iterator it = surfaces.begin();
         it != surfaces.end(); ++it) {
      it->y += y_offset;
    }

    // 描画サーフェス再描画のための、全消去
    eraseAllSurfaces();

    // 最も古いサーフェス位置が範囲外になる場合に削除
    pImpl::y_info_t& last_line = surfaces.front();
    if (((last_line.y + font_size) > height) ||
        (last_line.y < 0)){
      // １行分のサーフェスを削除
      removeLine();
    }
  }

  void scroll(int mode = NormalScroll) {

    //size_t adjusted_next_y = next_y;

    if ((erase_type == FadeOut) && (mode == ForceScroll)) {
      // １行目以降の行をスクロールさせて、追加の文字を続けて描画させる
      // !!! 強制的にスクロール中の最後の next_y の値を保持しておく必要がある
    }

#if 0
    // 直前までが ForceDownScroll のときの処理
    if ((force_next_y == NoForcedPosition) && (mode == NormalScroll)) {
      force_next_y = NoFocedPosition;

      // 位置を戻す
    }

    // mode == ForceDownScroll のときは、次のカーソルが下になるようにする
    if (mode == ForceDownScroll) {

      // Down && ScrollOut のときは、
      // Down && FadeOut のときは、
      // !!! つか、そのあたりの細かい実装は今後でよかろ

      if (force_next_y == NoFocedPosition) {
        //force_next_y
        // !!!
      }
        // !!! FadeOut の場合、一時的に、next_y を所定値以外にするのかな？
    }

    // Down のときは、
#endif

    if (erase_type != ScrollOut) {
      // フェードのときは、改行によって必ず全体をスクロールする
      fadeOut();

    } else if (((direction == Down) && ((next_y + font_size) > height)) ||
               ((direction != Down) && (static_cast<int>(next_y) < 0))) {
      // スクロールアウトの場合は、行を追加すると範囲外になる場合にスクロール
      scrollOut();

    } else {
      // ここに入るのは、モードが ScrollOut でスクロールが必要ないとき
      next_y += font_size * ((direction == Down) ? +1 : -1);
    }

    // X 位置の更新
    next_x = 0;

    // Y 位置の更新
    y_info_t y_info;
    y_info.y = next_y;
    y_info.surfaces_size = 0;
    surfaces.push_back(y_info);

    position_changed = true;
  }

  void addCh(Uint16 ch) {
    //fprintf(stderr, "first: %d, ", surfaces_size);

    if (ch == ' ') {
      // 空白のときは、幅を空ける。半角を想定し、幅は半分にしてしまう
      next_x += font_size / 2;

    } else if ((ch == '\n') && (erase_type == ScrollOut)) {
      // 改行処理
      scroll();

    } else if ((ch == '\n') && (erase_type == FadeOut)) {
      // 改行予約
      next_scroll = true;

    } else {
      if ((erase_type == FadeOut) && (next_scroll)) {
        next_scroll = false;
        scroll();
      }

      // サーフェスの作成
      x_info_t x_info;
      x_info.registed_ticks = InitialTicks;
      Uint16 text[2] = { 0x0, 0x0 };
      text[0] = ch;

      // !!! 色を指定色に置き換える
      TextProperty text_property(ttf, text, font_size,
                                 fore_color, back_color, true);
      boost::shared_ptr<TextSurface> surface(new TextSurface(text_property));
      x_info.w = surface->getWidth();
      x_info.h = surface->getHeight();

      // !!! 厳密にやるなら、各行の文字中の最大高さを、その行の高さにすべき
      // !!! font_size を越えたときぐらいでよさそうな気もするが

      if ((next_x + x_info.w) > width) {
        // 改行処理
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

      // サーフェスの入れ物を追加
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

    // フェード処理
    // !!! 以下、ちゃんと動作したらでよいので、関数化しておく
    if (x_it->registed_ticks == pImpl::InitialTicks) {
      x_it->registed_ticks = ticks;

      return false;
    } else {
      // フェード効果の反映
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

      // 完全に透明になったので、サーフェスを削除する
      if (adjusted_percent == 0) {
        //fprintf(stderr, "remove: \n");
        // !!!
        // !!! 表示対象にしなければよい

        // !!! サーフェスの資源を解放する、という意味では、削除すべき
        // !!! 削除してみる
        // 削除は、最も古いサーフェスになるはず
        //removeLatestSurface();
        ++invisible_surfaces;

        return true;
      }
      // !!! いずれ、完全に透明になったら、サーフェス自体を消去すべき

      return true;
    }
  }

  // 最も古いサーフェスを削除
  void removeLatestSurface(void) {

    y_info_t& first_line = surfaces.front();
    // 行が空になるらば、削除してしまう
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

  // 完全に透明になったサーフェスの登録削除
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

      // position の更新
      set_SdlRect(&position,
                  pimpl->position.x + x_it->x,
                  pimpl->position.y + y_it->y, x_it->w, x_it->h);
      DrawSurface* p = pimpl->draw_surfaces.at(index);
      ++index;

      bool changed = false;
      if (pimpl->erase_type == FadeOut) {
        // 文字のフェードアウト
        // !!! changed が関係ないようなら、消す
        changed = pimpl->fadeOutSurface(x_it, ticks);
      }
      updateDrawSurface(*p, x_it->surface, area, ticks,
                        position, pimpl->position_changed | changed);

      // 描画サーフェスを登録
      surfaces.push_back(p);
    }
  }
  pimpl->position_changed = false;

  // 完全に透明になったサーフェスの削除
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
  // 何もしない
}


void ConsolePanelComponent::registerLayer(Layer* layer) {
  pimpl->layer = layer;
}


int ConsolePanelComponent::printf(const char *fmt, ...) {
  if (! pimpl->layer) {
    // 登録されてないときの操作は、無視する
    return -1;
  }

  va_list ap;
  va_start(ap, fmt);

  return pimpl->printf(fmt, ap);
}


/*!
  \brief 表示内容の消去

  \attention まだ正常に動作しない
*/
void ConsolePanelComponent::clear(void) {

  // サーフェスを削除
  pimpl->eraseAllSurfaces();
  pimpl->draw_surfaces.clear();

  // 初期状態に更新
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
