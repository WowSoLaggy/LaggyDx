#include "stdafx.h"
#include "Button.h"

#include "App.h"
#include "IFontResource.h"


namespace Dx
{
  void Button::setTextureName(ButtonState i_state, std::string i_textureName)
  {
    d_textures[i_state] = std::move(i_textureName);
    if (i_state == d_state)
      updateTexture();
  }


  void Button::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());
    i_renderer.renderSprite(d_sprite);

    d_text.render(i_renderer, getPositionAbsolute() + d_textOffset);
  }


  void Button::onMouseMove()
  {
    const auto& mousePos = App::get().getInputDevice().getMousePosition();
    if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
    {
      if (d_state == ButtonState::Normal)
        setState(ButtonState::Hover);
    }
    else
    {
      if (d_state == ButtonState::Hover)
        setState(ButtonState::Normal);
    }
  }

  void Button::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
    {
      const auto& mousePos = App::get().getInputDevice().getMousePosition();
      if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
        setState(ButtonState::Pressed);
    }
  }

  void Button::onMouseRelease(MouseKey i_key)
  {
    if (i_key == MouseKey::Left && d_state == ButtonState::Pressed)
    {
      const auto& mousePos = App::get().getInputDevice().getMousePosition();
      if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
      {
        setState(ButtonState::Hover);
        onPress();
      }
      else
        setState(ButtonState::Normal);
    }
  }


  void Button::setText(std::string i_text)
  {
    d_text.setText(i_text);
    updateTextOffset();
  }

  void Button::setFont(const std::string& i_fontName)
  {
    d_text.setFont(i_fontName);
    updateTextOffset();
  }


  void Button::setTextScale(const float i_scale)
  {
    d_text.setScale(i_scale);
    updateTextOffset();
  }

  float Button::getTextScale() const
  {
    return d_text.getScale();
  }


  void Button::onPress()
  {
    if (d_onPress)
      d_onPress();
  }

  void Button::setOnPress(std::function<void()> i_onPress)
  {
    d_onPress = i_onPress;
  }


  void Button::updateTexture()
  {
    const auto& rc = App::get().getResourceController();
    d_sprite.setTexture(rc.getTexture(d_textures[d_state]));
    d_sprite.resetSizeToTexture();

    setSize(d_sprite.getSize().getVector<float>());

    updateTextOffset();
  }

  void Button::updateTextOffset()
  {
    if (d_text.getText().empty() || !d_text.getFontResource())
      return;

    const auto textSize = d_text.getSize();
    d_textOffset = {
      (float)((float)d_sprite.getSize().x - textSize.x) / 2,
      (float)((float)d_sprite.getSize().y - textSize.y) / 2
    };
  }

  void Button::setState(ButtonState i_state)
  {
    d_state = i_state;
    updateTexture();
  }


} // ns Dx
