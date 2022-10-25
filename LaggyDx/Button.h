#pragma once

#include "Control.h"
#include "Text.h"
#include "Sprite.h"


namespace Dx
{
  enum class ButtonState
  {
    Normal,
    Hover,
    Pressed,
  };

  class Button : public Control
  {
  public:
    void setTextureName(ButtonState i_state, std::string i_textureName);

    virtual void render(IRenderer2d& i_renderer) const override;

    virtual void onMouseMove() override;
    virtual void onMouseClick(MouseKey i_key) override;
    virtual void onMouseRelease(MouseKey i_key) override;

    void setText(std::string i_text);
    void setFont(const std::string& i_fontName);

    void setTextScale(float i_scale);
    float getTextScale() const;

    void onPress();
    void setOnPress(std::function<void()> i_onPress);

  private:
    Sprite d_sprite;
    Text d_text;

    ButtonState d_state = ButtonState::Normal;
    std::unordered_map<ButtonState, std::string> d_textures;

    Sdk::Vector2F d_textOffset;

    std::function<void()> d_onPress;

    void updateTexture();
    void updateTextOffset();
    void setState(ButtonState i_state);
  };

} // ns Dx