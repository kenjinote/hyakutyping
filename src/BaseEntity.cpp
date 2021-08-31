/*!
  \file
  \brief エンティティの基本クラス

  \author Satofumi KAMIMURA

  $Id$

  「実例で学ぶゲーム AI プログラム」(O'REILLY) より

  \todo LogManager を用いてエラー状態を記録する
*/

#include "BaseEntity.h"
#include <cstdio>

using namespace beego;


struct BaseEntity::pImpl {
  static int next_entity_id;
  int entity_id;

  pImpl(int id) {
    if (id < next_entity_id) {
      // ID の一意性を保証する
      // !!! LogManager に記録するように変更する
      fprintf(stderr, "invalid ID\n");
      return;
    }
    next_entity_id = id;
  }
};
int BaseEntity::pImpl::next_entity_id = 0;


BaseEntity::BaseEntity(int id) : pimpl(new pImpl(id)) {
}


BaseEntity::~BaseEntity(void) {
}


int BaseEntity::getId(void) const {
  return pimpl->entity_id;
}
