#include "stdafx.h"
#include "Button.h"

#include "Game.h"
#include "IFontResource.h"


namespace Dx
{
  void Button::setTextureName(ButtonState i_state, std::string i_textureName)
  {
    d_textures[i_state] = std::move(i_textureName);
    if (i_state == d_state)
      updateTexture();
  }


  void Button::setPosition(Sdk::Vector2F i_position)
  {
    Label::setPosition(i_position);
    d_sprite.setPosition({ (int)i_position.x, (int)i_position.y });
  }

  void Button::setText(std::string i_text)
  {
    Label::setText(i_text);
    updateTextOffset();
  }

  Sdk::Vector2F Button::getSize() const
  {
    const auto spriteSize = d_sprite.getSize();
    return { (float)spriteSize.x, (float)spriteSize.y };
  }

  void Button::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    const auto translationOgirinal = i_renderer.getTranslation();

    i_renderer.setTranslation(translationOgirinal + i_parentPos);
    i_renderer.renderSprite(d_sprite);
    i_renderer.setTranslation(translationOgirinal);

    Label::render(i_renderer, i_parentPos + d_textOffset);
  }


  void Button::onMouseMove()
  {
    const auto mousePos = Game::get().getInputDevice().getMousePosition();
    if (getRect().getRect<int>().containsPoint(mousePos))
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
      const auto mousePos = Game::get().getInputDevice().getMousePosition();
      if (getRect().getRect<int>().containsPoint(mousePos))
        setState(ButtonState::Pressed);
    }
  }

  void Button::onMouseRelease(MouseKey i_key)
  {
    if (i_key == MouseKey::Left && d_state == ButtonState::Pressed)
    {
      const auto mousePos = Game::get().getInputDevice().getMousePosition();
      if (getRect().getRect<int>().containsPoint(mousePos))
      {
        setState(ButtonState::Hover);
        onPress();
      }
      else
        setState(ButtonState::Normal);
    }
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
    auto& rc = Game::get().getResourceController();
    d_sprite.setTexture(rc.getTextureResource(d_textures[d_state]));
    d_sprite.resetSizeToTexture();

    updateTextOffset();
  }

  void Button::updateTextOffset()
  {
    const auto selfSize = d_sprite.getSize();
    const auto textSize = getFontResource()->getStringRect(getText());
    d_textOffset = { (float)(selfSize.x - textSize.width()) / 2, (float)(selfSize.y - textSize.height()) / 2 };
  }

  void Button::setState(ButtonState i_state)
  {
    d_state = i_state;
    updateTexture();
  }


} // ns Dx
