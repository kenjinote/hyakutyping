#ifndef UPDATE_UTILS_H
#define UPDATE_UTILS_H

/*!
  \file
  \brief メニューの更新補助

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>

class CommonResources;


class UpdateUtils {
  UpdateUtils(void);
  UpdateUtils(const UpdateUtils& rhs);
  UpdateUtils& operator = (const UpdateUtils& rhs);

  struct pImpl;
  std::auto_ptr<pImpl> pimpl;

public:
  UpdateUtils(CommonResources* common);
  ~UpdateUtils(void);

  bool update(void);
  bool haveQuitEvent(void);
};

#endif /* !UPDATE_UTILS_H */
