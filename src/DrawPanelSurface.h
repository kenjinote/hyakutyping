#ifndef DRAW_PANEL_SURFACE_H
#define DRAW_PANEL_SURFACE_H

/*!
  \file
  \brief �`��p�l���R���T�[�t�F�X

  \author Satofumi KAMIMURA

  $Id$

  \todo �C���^�[�t�F�[�X�� GlDrawPanelSurface.h �ɍ��킹��
*/

#include "SurfaceInterface.h"
#include "GridTypes.h"
#include "PlaceOption.h"
#include <memory>


namespace beego {
  //class TtfResource;
  class TextProperty;

  /*!
    \brief �}�`�̕`��T�[�t�F�X
  */
  class DrawPanelSurface : public SurfaceInterface {
    DrawPanelSurface(void);
    DrawPanelSurface(const DrawPanelSurface& rhs);
    DrawPanelSurface& operator = (const DrawPanelSurface& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    DrawPanelSurface(size_t width, size_t height);
    ~DrawPanelSurface(void);

    void draw(std::vector<SDL_Rect>& update_rects,
              const SDL_Rect* pos, const SDL_Rect* area);
    size_t getWidth(void);
    size_t getHeight(void);
    void forceSetChanged(void);
    bool isChanged(size_t ticks);
    bool isTransparent(void);

    void clear(void);

    //void setFontResource(TtfResource* font);
    void setBackgroundColor(Uint32 color);
    void setNoBackground(void);
    void drawPoint(const Grid<int>& point, Uint32 color);
    void drawPoints(const std::vector<Grid<int> >& points, Uint32 color);
    void drawLine(const Grid<int>& p1, const Grid<int>& p2, Uint32 color);
    void drawLines(const std::vector<Grid<int> >& points, Uint32 color);

    // !!! ����A���̃��\�b�h���͂Ȃ�����...
    void drawRect(const Grid<int>& position, size_t w, size_t h, Uint32 color);

    void drawText(const Grid<int>& position,
                  const TextProperty& text, int mode = None);
  };
};

#endif /* !DRAW_PANEL_SURFACE_H */
