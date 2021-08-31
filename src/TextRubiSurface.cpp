/*!
  \file
  \brief 文字列のルビサーフェス作成

  \author Satofumi KAMIMURA

  $Id$

  \todo 幅をはみ出てルビが振られることがあるのを修正すべき
*/

#include "TextRubiSurface.h"
#include "TextFadeSurface.h"
#include "TextProperty.h"
#include "TextSurface.h"
#include "UtfString.h"

using namespace beego;


struct TextRubiSurface::pImpl {

  class RubiSurface : public FadeSurfacesInterface {
    FadeSurfaces surfaces_;
    std::vector<int> ch_pos_;
    size_t width_;
    size_t height_;
    int total_size_;

  public:
    int x_offset_;

  private:
    void createSurface(std::vector<Uint16>& rubi_add, size_t rubi_size,
                       const TextProperty& property,
                       int first_pos, int last_pos) {
      int kanji_size = property.size;

      if (rubi_add.empty()) {
        return;
      }

      Uint16 rubi_ch[2] = { 0x0, 0x0 };
      size_t kanji_width = kanji_size * (last_pos - first_pos);
      size_t each_ch = kanji_width / rubi_add.size();
      if (each_ch < rubi_size) {
        each_ch = rubi_size;
      }
      int x_offset =
        kanji_width - (((rubi_add.size() - 1) * each_ch) + rubi_size);
      int index = 0;
      for (std::vector<Uint16>::iterator it = rubi_add.begin();
           it != rubi_add.end(); ++it, ++index) {
        rubi_ch[0] = *it;

        TextProperty text_property(property.font, rubi_ch, rubi_size,
                                   property.fore_color, property.back_color,
                                   property.transparent);
        boost::shared_ptr<TextSurface> surface(new TextSurface(text_property));

        int x = x_offset/2 + (each_ch * index);
        if (x < x_offset_) {
          x_offset_ = x;
        }

        ch_pos_.push_back((100 * (kanji_size * first_pos + x)) / total_size_);
        height_ =
          (height_ > surface->getWidth()) ? height_ : surface->getWidth();
        surfaces_.push_back(surface);
      }
      rubi_add.clear();
    }

    void createRubiSurfaces(const Uint16* kana_text, size_t rubi_size,
                            const TextProperty& property) {

      const Uint16* kanji_text = property.utext;

      const Uint16* kanji_ch = kanji_text;
      const Uint16* kanji_first_ch = kanji_ch;
      const Uint16* rubi_ch = kana_text;

      // 最後の平仮名部分を無視するようにする
      int kanji_index_last = ustrlen(kanji_text);
      int kana_index_last = ustrlen(kana_text);

      while ((kanji_text[kanji_index_last] == kana_text[kana_index_last]) &&
             (kanji_index_last > 0) && (kana_index_last> 0)) {
        --kanji_index_last;
        --kana_index_last;
      }

      // マッチしない文字の箇所を探す
      for (; (*kanji_ch && *rubi_ch) &&
             ((kanji_ch - kanji_text) <= kanji_index_last) &&
             ((rubi_ch - kana_text) <= kana_index_last);
           ++kanji_ch) {
        if (*kanji_ch == *rubi_ch) {
          ++rubi_ch;
          ++kanji_first_ch;
          continue;
        }

        // ルビの範囲を探す
        bool surface_created = false;
        for (const Uint16* kanji_end = kanji_ch + 1;
             ((kanji_end - kanji_text) <= kanji_index_last) && *kanji_end;
             ++kanji_end) {
          for (const Uint16* rubi_end = rubi_ch;
               ((rubi_end - kana_text ) <= kana_index_last) && *rubi_end;
               ++rubi_end) {
            if (*rubi_end == *kanji_end) {
              // サーフェスの作成
              std::vector<Uint16> rubi_add;
              size_t rubi_first = rubi_ch - kana_text;
              size_t rubi_last = rubi_end - kana_text;
              for (size_t i = rubi_first; i < rubi_last; ++i) {
                rubi_add.push_back(kana_text[i]);
              }
              size_t kanji_first = kanji_ch - kanji_text;
              size_t kanji_last = kanji_end - kanji_text;
              size_t kanji_length = kanji_last - kanji_first;

              // 文字数が１文字で、ひらがなの場合、るびは１字までとする
              // あ:3041 - ん:3093, ゐ:3090, ゑ:3091
              if ((kanji_length == 1) && (rubi_add.size() > 1)) {
                Uint16 ch = *kanji_ch;
                if ((ch >= 0x3041) && (ch <= 0x3093) &&
                    (ch != 0x3090) && (ch != 0x3091)) {
                  continue;
                }
              }

              // るび文字数が漢字より小さければ、無効扱いにする
              if (kanji_length > rubi_add.size()) {
                continue;
              }

              rubi_ch += rubi_last - rubi_first;
              kanji_first_ch = kanji_text + kanji_last;
              createSurface(rubi_add, rubi_size, property,
                            kanji_first, kanji_last);
              kanji_ch += kanji_last - kanji_first -1;
              surface_created = true;
              break;
            }
          }
          if (surface_created) {
            break;
          }
        }
      }

      // 最後まで残った文字を、ルビとして扱う
      if (((rubi_ch - kana_text) <= kana_index_last) && *rubi_ch) {
        std::vector<Uint16> rubi_add;
        size_t n = kana_index_last - (rubi_ch - kana_text) + 1;
        for (size_t i = 0; i < n; ++i) {
          rubi_add.push_back(*(rubi_ch + i));
        }
        size_t kanji_first = kanji_first_ch - kanji_text;
        n = kanji_index_last - kanji_first + 1;
        createSurface(rubi_add, rubi_size, property,
                      kanji_first, kanji_first + n);
      }
    }

  public:
    RubiSurface(const Uint16* kana_text, size_t rubi_size,
                const TextProperty& property)
      : width_(0), height_(0), x_offset_(0) {

      total_size_ = ustrlen(property.utext) * property.size;
      width_ = total_size_;

      // ルビ文字サーフェス、および位置の決定
      createRubiSurfaces(kana_text, rubi_size, property);

      // !!! とりあえず、高さが 0 だったら、幅も 0 にしておく
      // !!! この実装で適切かは、不明
      if (height_ == 0) {
        width_ = 0;
      }
    }

    ~RubiSurface(void) {
    }

    FadeSurfaces& getSurfaces(void) {
      return surfaces_;
    }

    int getPositionPercent(size_t index) {
      return static_cast<int>(ch_pos_[index] - (100 * x_offset_ / total_size_));
    }

    size_t getWidth(void) {
      return width_;
    }

    size_t getHeight(void) {
      return height_;
    }
  };

  enum {
    DefaultFadeNum = 3,
  };
  RubiSurface rubi_obj;
  TextFadeSurface text_fade;
  size_t width;
  size_t height;

  pImpl(const Uint16* kana_text, size_t rubi_size,
        const TextProperty& property)
    : rubi_obj(kana_text, rubi_size, property),
      text_fade(&rubi_obj, property.size * DefaultFadeNum),
      width(rubi_obj.getWidth()), height(rubi_obj.getHeight()) {
    text_fade.setFadePercent(100);
  }
};


TextRubiSurface::TextRubiSurface(const Uint16* kana_text, size_t rubi_size,
                                 const TextProperty& property)
  : pimpl(new pImpl(kana_text, rubi_size, property)) {
}


TextRubiSurface::~TextRubiSurface(void) {
}


void TextRubiSurface::draw(std::vector<SDL_Rect>& update_rects,
                           const SDL_Rect* pos, const SDL_Rect* area) {
  pimpl->text_fade.draw(update_rects, pos, area);
}


size_t TextRubiSurface::getWidth(void) {
  // !!! *2 は、適当...
  return pimpl->width - (pimpl->rubi_obj.x_offset_ * 2);
}


size_t TextRubiSurface::getHeight(void) {
  return pimpl->height;
}


void TextRubiSurface::forceSetChanged(void) {
  pimpl->text_fade.forceSetChanged();
}


bool TextRubiSurface::isChanged(size_t ticks) {
  return pimpl->text_fade.isChanged(ticks);
}


bool TextRubiSurface::isTransparent(void) {
  return pimpl->text_fade.isTransparent();
}


void TextRubiSurface::setFadeMode(FadeMode mode, size_t width) {
  pimpl->text_fade.setFadeMode(mode, width);
}


void TextRubiSurface::setFadePercent(size_t percent) {
  pimpl->text_fade.setFadePercent(percent);
}


int TextRubiSurface::getDrawOffset(void) {
  return pimpl->rubi_obj.x_offset_;
}
