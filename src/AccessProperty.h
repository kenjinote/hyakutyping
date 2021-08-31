#ifndef ACCESS_PROPERTY_H
#define ACCESS_PROPERTY_H

/*!
  \file
  \brief ëÆê´èÓïÒÇÃì«Ç›èoÇµÇ∆ïœçX

  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <string>


namespace beego {
  class AccessProperty {
    AccessProperty(void);
    AccessProperty(const AccessProperty& rhs);
    AccessProperty& operator = (const AccessProperty& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    AccessProperty(const char* fname);
    ~AccessProperty(void);
    const char* what(void);

    std::string& operator[](const char* key) const;

    bool reload(void);
    bool save(void);
    bool isChangedByOther(void);
  };
};

#endif /*! ACCESS_PROPERTY_H */
