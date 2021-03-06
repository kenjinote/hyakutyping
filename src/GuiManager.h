#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

/*!
  \file
  \brief GUI 管理

  \author Satofumi KAMIMURA

  $Id$
*/

#include <SDL.h>
#include <memory>

namespace beego {
  class Layer;
  class InputEvent;

  class GuiManager {
    GuiManager(const GuiManager& rhs);
    GuiManager& operator = (const GuiManager& rhs);

    struct pImpl;
    //const std::auto_ptr<pImpl> pimpl;
    pImpl* pimpl;

  public:
    typedef enum {
      User,
      Auto,
    } RedrawMode;
    GuiManager(void);
    GuiManager(RedrawMode mode);
    ~GuiManager(void);

    void push_front(Layer* layer);
    void push_back(Layer* layer);
    void insert(const Layer* dst, Layer* layer);
    void remove(Layer* layer);

    // !!! 入力の評価を行い、再描画を行うかも
    void update(void);

    // !!! 入力の評価を行い、再描画を行う
    void redraw(void);
  };
};

#endif /* !GUI_MANAGER_H */
