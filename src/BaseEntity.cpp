/*!
  \file
  \brief �G���e�B�e�B�̊�{�N���X

  \author Satofumi KAMIMURA

  $Id$

  �u����Ŋw�ԃQ�[�� AI �v���O�����v(O'REILLY) ���

  \todo LogManager ��p���ăG���[��Ԃ��L�^����
*/

#include "BaseEntity.h"
#include <cstdio>

using namespace beego;


struct BaseEntity::pImpl {
  static int next_entity_id;
  int entity_id;

  pImpl(int id) {
    if (id < next_entity_id) {
      // ID �̈�Ӑ���ۏ؂���
      // !!! LogManager �ɋL�^����悤�ɕύX����
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
