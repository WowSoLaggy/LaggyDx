#include "stdafx.h"
#include "Button.h"

#include "App.h"
#include "ControlEvents.h"
#include "IFontResource.h"
#include "ImageDescription.h"
#include "ITexture.h"


namespace Dx
{
  void Button::setTextureName(const std::string& i_textureName)
  {
    for (int i = 0; i < static_cast<int>(ButtonState::Count); ++i)
      d_textures[static_cast<ButtonState>(i)] = i_textureName;
    
    updateTexture();
  }

  void Button::setTextureName(const std::string& i_textureName, const ButtonState i_state)
  {
    d_textures[i_state] = i_textureName;
    if (i_state == d_state)
      updateTexture();
  }

  void Button::setSize(Sdk::Vector2F i_size)
  {
    Control::setSize(i_size);
    d_sprite.setSize(std::move(i_size));
    updateTextOffset();
  }

  void Button::resetSizeToTexture()
  {
    const auto textureSize = d_sprite.getTexture() ? d_sprite.getTexture()->getDescription().frameSize : Sdk::Vector2I{ 0, 0 };
    setSize(textureSize.getVector<float>());
  }


  void Button::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());
    i_renderer.renderSprite(d_sprite);

    d_text.render(getPositionAbsolute() + d_textOffset);
  }


  void Button::onMouseMove()
  {
    const auto& mousePos = App::get().getInputDevice().getMousePosition();
    if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
    {
      if (d_state == ButtonState::Normal)
        setState(ButtonState::Hovered);
    }
    else
    {
      if (d_state == ButtonState::Hovered)
        setState(ButtonState::Normal);
    }
  }

  bool Button::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
    {
      const auto& mousePos = App::get().getInputDevice().getMousePosition();
      if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
      {
        setState(ButtonState::Pressed);
        return true;
      }
    }

    return Control::onMouseClick(i_key);
  }

  bool Button::onMouseRelease(MouseKey i_key)
  {
    if (i_key == MouseKey::Left && d_state == ButtonState::Pressed)
    {
      const auto& mousePos = App::get().getInputDevice().getMousePosition();
      if (getRectAbsolute().getRect<int>().containsPoint(mousePos))
      {
        setState(ButtonState::Hovered);
        onPress();
      }
      else
        setState(ButtonState::Normal);

      return true;
    }

    return false;
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


  void Button::setTextAlignment(const TextAlignment i_alignment)
  {
    d_textAlignment = i_alignment;
    updateTextOffset();
  }

  void Button::setBorderTextOffset(const int i_offset)
  {
    d_borderTextOffset = i_offset;
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
    {
      d_onPress();
      notify(ButtonPressedEvent());
    }
  }

  void Button::setOnPress(std::function<void()> i_onPress)
  {
    d_onPress = i_onPress;
  }


  void Button::updateTexture()
  {
    const auto& rc = App::get().getResourceController();
    d_sprite.setTexture(rc.getTexture(d_textures[d_state]));
  }

  void Button::updateTextOffset()
  {
    if (d_text.getText().empty() || !d_text.getFontResource())
      return;

    const auto textSize = d_text.getSize();

    switch (d_textAlignment)
    {
    case TextAlignment::Left:
      d_textOffset = { (float)d_borderTextOffset, (float)((float)getSize().y - textSize.y) / 2 };
      break;
    case TextAlignment::Center:
      d_textOffset = {
        std::round(((float)getSize().x - textSize.x) / 2),
        std::round(((float)getSize().y - textSize.y) / 2)
      };
      break;
    case TextAlignment::Right:
      d_textOffset = { (float)getSize().x - textSize.x - d_borderTextOffset, (float)((float)getSize().y - textSize.y) / 2 };
      break;
    }
  }

  void Button::setState(ButtonState i_state)
  {
    d_state = i_state;
    updateTexture();
  }


} // ns Dx
