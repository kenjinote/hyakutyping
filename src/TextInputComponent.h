#ifndef TEXT_INPUT_COMPONENT_H
#define TEXT_INPUT_COMPONENT_H

/*!
  \file
  \brief ���͕����̕\���R���|�[�l���g

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ComponentInterface.h"
#include "GuiColors.h"
#include <memory>

class TextConvertInterface;


namespace beego {
  class TtfResource;
  class TextProperty;

  class TextInputComponent : public ComponentInterface {
    TextInputComponent(const TextInputComponent& rhs);
    TextInputComponent& operator = (const TextInputComponent& rhs);

    struct pImpl;
    const std::auto_ptr<pImpl> pimpl;

  public:
    TextInputComponent(size_t width, size_t height,
                       const TextProperty& property,
                       int x_offset = 0, int y_offset = 0);
    ~TextInputComponent(void);

    void addSurfaceList(SurfaceList& surfaces, const SDL_Rect* area,
                        size_t ticks);
    void setPosition(const SDL_Rect* position);
    void getPosition(SDL_Rect* position);
    size_t getWidth(void);
    size_t getHeight(void);
    void applyInput(const InputEvent& event,
                    std::vector<SDL_Rect>& applied_rects);
    void registerLayer(Layer* layer);

    // !!! ���͂��󂯕t����A�󂯕t���Ȃ��A�̐ݒ�

    bool isDecided(void);
    void releaseDecided(void);
    void clear(void);

    /*!
      \brief ���͕�����̎擾

      \attention GuiManager::getMutex() ��p�������b�N�ɂ��r�����䂪�K�v

      �g�p��
      \code
      // !!! �܂�
      \endcodd
    */
    size_t getBuffer(std::vector<Uint16>& buffer);
    void setBuffer(std::vector<Uint16>& buffer);

    void swapTextConverter(TextConvertInterface* converter);

    bool empty(void);

    bool isChanged(void);

    bool isErased(void);

    // !!! �J�[�\���T�[�t�F�X�̓o�^
  };
};

#endif /* !TEXT_INPUT_COMPONENT_H */
