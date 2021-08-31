#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

/*!
  \file
  \brief エンティティの基本クラス

  \author Satofumi KAMIMURA

  $Id$

  「実例で学ぶゲーム AI プログラム」(O'REILLY) より
*/

#include <memory>


namespace beego {
  class BaseEntity {
    BaseEntity(void);
    BaseEntity(const BaseEntity& rhs);
    BaseEntity& operator = (const BaseEntity& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    BaseEntity(int id);
    virtual ~BaseEntity(void);

    virtual void update(void) = 0;
    int getId(void) const;
  };
};

#endif /* !BASE_ENTITY_H */
