#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

/*!
  \file
  \brief �{�^���R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include <memory>

namespace beego {
  class TtfResource;
  class TextProperty;

  class ButtonComponent : public ComponentInterface {
    ButtonComponent(const ButtonComponent& rhs);
    ButtonComponent& operator = (const ButtonComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    enum {
      NoSelect = -1,
    };
    ButtonComponent(void);
    ~ButtonComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! �傫���̈Ⴄ�T�[�t�F�X��o�^�������ꍇ�ɂ́A�ǂ�����̂��ȁH
    // !!! - �傫���͓����ɂ���
    // !!! - �ő�T�C�Y���w�肵�Ă����A�T�[�t�F�X�ƈʒu���w��ł���悤�ɂ���
    // !!! - �قȂ�T�[�t�F�X�ł��A���Ȃ��ƌ��Ȃ�
    // !!!   - �T�[�t�F�X�̊�ʒu���w�肷��
    // !!!     - ���S��A�������A�E�����A���㉺�A���E����

    // !!! �`��T�[�t�F�X�T�C�Y�ƁA����̔z�u�I�t�Z�b�g���w��\�ɂ���

    // ��T�[�t�F�X��o�^
    void registerNormalSurface(Surface surface,
                               int x_offset =0, int y_offset = 0);

    // �������T�[�t�F�X��o�^
    void registerPressedSurface(Surface surface,
                                int x_offset =0, int y_offset = 0);

    // �t�H�[�J�X���T�[�t�F�X��o�^
    void registerFocusedSurface(Surface surface,
                                int x_offset =0, int y_offset = 0);

    Surface getNormalSurface(void);

    void showNormalSurface(void);
    void showPressedSurface(void);
    void showFocusedSurface(void);

    // !!! �\���T�[�t�F�X�����[�U���w�肷�邩�A�L�[�Ŏw�肷�邩�A��I��������

    bool isChanged(void);
    bool isDecided(void);
    void setDecided(void);
    void releaseDecided(void);

    static void createButton(boost::shared_ptr<ButtonComponent>& button,
                             const TextProperty& normal,
                             const TextProperty& selected,
                             const TextProperty& pressed);
  };
};

#endif /* !BUTTON_COMPONENT_H */
