/*!
  \file
  \brief TTF �����̊Ǘ�

  \author Satofumi KAMIMURA

  $Id$

  \todo �t�@�C���`�F�b�N�ɂ� FindFile �Ȃǂ𗘗p���ׂ�
*/

#include "TtfResource.h"
#include "TtfInit.h"
#include "DetectOS.h"
#ifdef MSC
#include <windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include <map>

#ifdef MSC
#define F_OK 06
#endif

using namespace beego;


/*!
  \brief TtfResource �̓����N���X
*/
struct TtfResource::pImpl : private TtfInit {
  const char* ttf_file;

  typedef std::map<size_t, TTF_Font*> FontMap;
  FontMap fonts;

  pImpl(const char* fname) : ttf_file(fname) {
    // ttf_file �����݂��邩�ǂ����̃`�F�b�N
    if (access(ttf_file, F_OK) < 0) {
      // !!! �G���[�̃��O�o��
      fprintf(stderr, "TTF file is not found: %s\n", ttf_file);
      return;
    }
  }

  ~pImpl(void) {
#if 0
    // ��I�u�W�F�N�g��������Ă��܂����Ƃ�����
    for (FontMap::iterator it = fonts.begin(); it != fonts.end(); ++it) {
      TTF_CloseFont(it->second);
    }
#endif
  }
};


TtfResource::TtfResource(const char* ttf_file) : pimpl(new pImpl(ttf_file)) {
}


TtfResource::~TtfResource(void) {
}


TTF_Font* TtfResource::getResource(size_t size) {
  pImpl::FontMap::iterator p = pimpl->fonts.find(size);
  if (p != pimpl->fonts.end()) {
    return p->second;
  }

  // ���݂��Ȃ��t�H���g�T�C�Y�̏ꍇ�ɁA�t�H���g�����𐶐�����
  TTF_Font* fp = TTF_OpenFont(pimpl->ttf_file, static_cast<int>(size));
  //fprintf(stderr, "%s: %p\n", pimpl->ttf_file, fp);
  if (fp == NULL) {
    // !!! error_message
    return NULL;
  }

  pimpl->fonts.insert(std::pair<size_t, TTF_Font*>(size, fp));
  return fp;
}
