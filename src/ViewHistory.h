#ifndef VIEW_HISTORY_H
#define VIEW_HISTORY_H

/*!
  \file
  \brief —š—ð‚Ì‰{——

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>


class ViewHistory {
  ViewHistory(const ViewHistory& rhs);
  ViewHistory& operator = (const ViewHistory& rhs);

  struct pImpl;
  const std::auto_ptr<pImpl> pimpl;

public:
  ViewHistory(void);
  ~ViewHistory(void);

  /*!
    \brief ‰{——‚ÌŠJŽn
  */
  void run(void);
};

#endif /* !VIEW_HISTORY_H */
