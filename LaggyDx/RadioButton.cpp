#include "stdafx.h"
#include "RadioButton.h"

#include "App.h"
#include "ControlEvents.h"


namespace Dx
{
  void RadioButton::setTextureName(RadioButtonState i_state, std::string i_textureName)
  {
    d_textures[i_state] = std::move(i_textureName);
    if (i_state == d_state)
      updateTexture();
  }


  void RadioButton::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());
    i_renderer.renderSprite(d_sprite);

    d_text.render(i_renderer, getPositionAbsolute() + d_textOffset);
  }


  void RadioButton::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
    {
      const auto& mousePos = App::get().getInputDevice().getMousePosition();
      if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
        check();
    }
  }


  void RadioButton::check()
  {
    setState(RadioButtonState::Checked);
    notify(RadioButtonCheckEvent(*this));
    onCheck();
  }

  void RadioButton::uncheck()
  {
    setState(RadioButtonState::Unchecked);
  }


  bool RadioButton::isChecked() const
  {
    return d_state == RadioButtonState::Checked;
  }


  void RadioButton::onCheck()
  {
    if (d_onCheck)
      d_onCheck();
  }

  void RadioButton::setOnCheck(OnRadioButtonCheckHandler i_onCheck)
  {
    d_onCheck = i_onCheck;
  }


  void RadioButton::setText(std::string i_text)
  {
    d_text.setText(i_text);
    updateTextOffset();
  }

  void RadioButton::setFont(const std::string& i_fontName)
  {
    d_text.setFont(i_fontName);
    updateTextOffset();
  }


  void RadioButton::setTextScale(const float i_scale)
  {
    d_text.setScale(i_scale);
    updateTextOffset();
  }

  float RadioButton::getTextScale() const
  {
    return d_text.getScale();
  }


  void RadioButton::updateTexture()
  {
    if (d_textures[d_state].empty())
      return;

    const auto& rc = App::get().getResourceController();
    d_sprite.setTexture(rc.getTexture(d_textures[d_state]));
    d_sprite.resetSizeToTexture();

    setSize(d_sprite.getSize().getVector<float>());
  }

  void RadioButton::updateTextOffset()
  {
    if (d_text.getText().empty() || !d_text.getFontResource())
      return;

    const auto textSize = d_text.getSize();
    d_textOffset = {
      (float)((float)d_sprite.getSize().x - textSize.x) / 2,
      (float)((float)d_sprite.getSize().y - textSize.y) / 2
    };
  }

  void RadioButton::setState(RadioButtonState i_state)
  {
    d_state = i_state;
    updateTexture();
  }

} // ns Dx
