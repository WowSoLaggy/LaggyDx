#include "stdafx.h"
#include "Checkbox.h"

#include "Game.h"


namespace Dx
{
  void Checkbox::setTextureName(CheckboxState i_state, std::string i_textureName)
  {
    d_textures[i_state] = std::move(i_textureName);
    if (i_state == d_state)
      updateTexture();
  }


  void Checkbox::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPosition());
    i_renderer.renderSprite(d_sprite);
  }


  void Checkbox::onMouseMove()
  {
    const auto mousePos = Game::get().getInputDevice().getMousePosition();
    if (getRect().getRect<int>().containsPoint(mousePos))
    {
      if (d_state == CheckboxState::Normal || d_state == CheckboxState::Ticked)
        setState(isChecked() ? CheckboxState::HoverTicked : CheckboxState::HoverUnticked);
    }
    else
    {
      if (d_state == CheckboxState::HoverTicked || d_state == CheckboxState::HoverUnticked)
        setState(isChecked() ? CheckboxState::Ticked : CheckboxState::Normal);
    }
  }

  void Checkbox::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
    {
      const auto mousePos = Game::get().getInputDevice().getMousePosition();
      if (getRect().getRect<int>().containsPoint(mousePos))
      {
        setState(isChecked() ? CheckboxState::HoverUnticked : CheckboxState::HoverTicked);
        onCheck();
      }
    }
  }


  bool Checkbox::isChecked() const
  {
    return d_state == CheckboxState::Ticked || d_state == CheckboxState::HoverTicked;
  }


  void Checkbox::onCheck()
  {
    if (d_onCheck)
      d_onCheck(isChecked());
  }

  void Checkbox::setOnCheck(std::function<void(bool)> i_onCheck)
  {
    d_onCheck = i_onCheck;
  }


  void Checkbox::updateTexture()
  {
    const auto& rc = Game::get().getResourceController();
    d_sprite.setTexture(rc.getTexture(d_textures[d_state]));
    d_sprite.resetSizeToTexture();

    setSize(d_sprite.getSize().getVector<float>());
  }

  void Checkbox::setState(CheckboxState i_state)
  {
    d_state = i_state;
    updateTexture();
  }

} // ns Dx
