#pragma once

#include "Control.h"
#include "AnimatedSprite.h"
#include "Text.h"


namespace Dx
{
  enum class ButtonState
  {
    Normal = 0,
    Hovered,
    Pressed,
    Disabled,

    Count
  };

  enum class TextAlignment
  {
    Left,
    Center,
    Right
  };

  class Button : public Control
  {
  public:
    void setTextureName(const std::string& i_textureName);
    void setTextureName(const std::string& i_textureName, ButtonState i_state);

    virtual void render(IRenderer2d& i_renderer) const override;

    virtual void onMouseMove() override;
    virtual bool onMouseClick(MouseKey i_key) override;
    virtual bool onMouseRelease(MouseKey i_key) override;

    virtual void setSize(Sdk::Vector2F i_size) override;
    void resetSizeToTexture();

    void setText(std::string i_text);
    void setFont(const std::string& i_fontName);

    void setTextAlignment(TextAlignment i_alignment);
    void setBorderTextOffset(int i_offset);

    void setTextScale(float i_scale);
    float getTextScale() const;

    void setColor(Sdk::Vector4F i_color);

    void setDisabled(bool i_disabled);
    bool isDisabled() const;

    void onPress();
    void setOnPress(std::function<void()> i_onPress);

  private:
    AnimatedSprite d_sprite;
    Text d_text;

    ButtonState d_state = ButtonState::Normal;
    std::unordered_map<ButtonState, std::string> d_textures;

    Sdk::Vector2F d_textOffset;
    int d_borderTextOffset = 10;
    TextAlignment d_textAlignment = TextAlignment::Center;

    std::function<void()> d_onPress;

    void updateTexture();
    void updateTextOffset();
    void setState(ButtonState i_state);
  };

} // ns Dx