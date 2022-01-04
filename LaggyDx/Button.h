#pragma once

#include "Label.h"
#include "Sprite.h"


namespace Dx
{
  enum class ButtonState
  {
    Normal,
    Hover,
    Pressed,
  };

  class Button : public Label
  {
  public:
    void setTextureName(ButtonState i_state, std::string i_textureName);

    virtual void setPosition(Sdk::Vector2F i_position) override;
    virtual void setText(std::string i_text) override;
    virtual Sdk::Vector2F getSize() const override;
    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;

    virtual void onMouseMove() override;
    virtual void onMouseClick(MouseKey i_key) override;
    virtual void onMouseRelease(MouseKey i_key) override;

    void onPress();
    void setOnPress(std::function<void()> i_onPress);

  private:
    Sprite d_sprite;

    ButtonState d_state = ButtonState::Normal;
    std::unordered_map<ButtonState, std::string> d_textures;

    Sdk::Vector2F d_textOffset;

    std::function<void()> d_onPress;

    void updateTexture();
    void updateTextOffset();
    void setState(ButtonState i_state);
  };

} // ns Dx