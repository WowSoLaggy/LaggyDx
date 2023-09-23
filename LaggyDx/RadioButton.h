#pragma once

#include "Control.h"
#include "Sprite.h"
#include "Text.h"


namespace Dx
{
  enum class RadioButtonState
  {
    Checked,
    Unchecked
  };

  using OnRadioButtonCheckHandler = std::function<void()>;

  class RadioButton : public Control
  {
  public:
    void setTextureName(RadioButtonState i_state, std::string i_textureName);

    virtual void render(IRenderer2d& i_renderer) const override;

    virtual bool onMouseClick(MouseKey i_key) override;

    void check();
    void uncheck();
    bool isChecked() const;

    void setOnCheck(OnRadioButtonCheckHandler i_onCheck);

    void setText(std::string i_text);
    void setFont(const std::string& i_fontName);

    void setTextScale(float i_scale);
    float getTextScale() const;

  private:
    Sprite d_sprite;
    Text d_text;

    RadioButtonState d_state = RadioButtonState::Checked;
    std::unordered_map<RadioButtonState, std::string> d_textures;

    Sdk::Vector2F d_textOffset;

    OnRadioButtonCheckHandler d_onCheck;

    void updateTexture();
    void updateTextOffset();
    void setState(RadioButtonState i_state);
    void onCheck();
  };

} // ns Dx
