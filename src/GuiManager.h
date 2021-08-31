#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

/*!
  \file
  \brief GUI ŠÇ—

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

    // !!! “ü—Í‚Ì•]‰¿‚ğs‚¢AÄ•`‰æ‚ğs‚¤‚©‚à
    void update(void);

    // !!! “ü—Í‚Ì•]‰¿‚ğs‚¢AÄ•`‰æ‚ğs‚¤
    void redraw(void);
  };
};

#endif /* !GUI_MANAGER_H */
