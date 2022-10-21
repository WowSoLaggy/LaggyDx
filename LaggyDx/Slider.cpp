#include "stdafx.h"
#include "Slider.h"

#include "Game.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  Slider::Slider()
  {
    setLength(128);
    setMinValue(0);
    setMaxValue(100);
    setCurrentValue(0);
  }


  void Slider::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());

    i_renderer.renderSprite(d_spriteLeftSide);
    i_renderer.renderSprite(d_spriteBack);
    i_renderer.renderSprite(d_spriteRightSide);
    i_renderer.renderSprite(d_spriteSlider);
  }


  Sdk::Vector2F Slider::getSize() const
  {
    return Sdk::Vector2I{
      d_spriteLeftSide.getSize().x + d_spriteBack.getSize().x + d_spriteRightSide.getSize().x,
      d_spriteSlider.getSize().y }.getVector<float>();
  }

  Sdk::Vector2I Slider::getSidesSize() const
  {
    return d_spriteLeftSide.getSize() + d_spriteRightSide.getSize();
  }


  void Slider::onMouseMove()
  {
    if (!d_isCurrentlyDragged)
      return;

    followMouse();
  }

  void Slider::onMouseClick(MouseKey i_key)
  {
    const auto mousePos = Game::get().getInputDevice().getMousePosition() - getPositionAbsolute().getVector<int>();
    
    if (d_spriteSlider.getRect().containsPoint(mousePos))
      d_isCurrentlyDragged = true;
    else if (d_spriteLeftSide.getRect().containsPoint(mousePos))
      setCurrentValue(d_minValue);
    else if (d_spriteRightSide.getRect().containsPoint(mousePos))
      setCurrentValue(d_maxValue);
    else if (d_spriteBack.getRect().containsPoint(mousePos))
    {
      d_isCurrentlyDragged = true;
      followMouse();
    }
  }

  void Slider::onMouseRelease(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
      d_isCurrentlyDragged = false;
  }


  void Slider::setTextureBack(const std::string& i_textureName)
  {
    const auto& rc = Game::get().getResourceController();
    d_spriteBack.setTexture(rc.getTexture(i_textureName));

    rearrange();
  }

  void Slider::setTextureSlider(const std::string& i_textureName)
  {
    const auto& rc = Game::get().getResourceController();
    d_spriteSlider.setTexture(rc.getTexture(i_textureName));
    d_spriteSlider.resetSizeToTexture();

    rearrange();
  }

  void Slider::setTextureRightSide(const std::string& i_textureName)
  {
    const auto& rc = Game::get().getResourceController();
    d_spriteRightSide.setTexture(rc.getTexture(i_textureName));
    d_spriteRightSide.resetSizeToTexture();

    rearrange();
  }

  void Slider::setTextureLeftSide(const std::string& i_textureName)
  {
    const auto& rc = Game::get().getResourceController();
    d_spriteLeftSide.setTexture(rc.getTexture(i_textureName));
    d_spriteLeftSide.resetSizeToTexture();

    rearrange();
  }


  void Slider::setLength(const int i_length)
  {
    d_length = i_length;
    rearrange();
  }


  void Slider::setMinValue(const double i_minValue)
  {
    d_minValue = i_minValue;

    if (d_maxValue <= d_minValue)
      d_maxValue = d_minValue + std::numeric_limits<double>::epsilon();

    updateSliderPosition();
  }

  void Slider::setMaxValue(const double i_maxValue)
  {
    d_maxValue = i_maxValue;

    if (d_minValue >= d_maxValue)
      d_minValue = d_maxValue - std::numeric_limits<double>::epsilon();

    updateSliderPosition();
  }

  void Slider::setCurrentValue(const double i_currentValue)
  {
    d_currentValue = Sdk::clamp(i_currentValue, d_minValue, d_maxValue);
    updateSliderPosition();
  }

  void Slider::setRelativeValue(double i_relativeValue)
  {
    i_relativeValue = Sdk::clamp(i_relativeValue, 0.0, 1.0);
    const double value = (d_maxValue - d_minValue) * i_relativeValue + d_minValue;
    setCurrentValue(value);
  }

  double Slider::getCurrentValue() const
  {
    return d_currentValue;
  }


  void Slider::rearrange()
  {
    d_spriteBack.resetSizeToTexture();
    d_spriteBack.setSize({ d_length, d_spriteBack.getSize().y });

    const int backStart = d_spriteLeftSide.getSize().x;
    const int backEnd = backStart + d_spriteBack.getSize().x;
    const int yOffset = (d_spriteSlider.getSize().y - d_spriteBack.getSize().y) / 2;

    d_spriteLeftSide.setPosition({ 0, yOffset });
    d_spriteBack.setPosition({ backStart, yOffset });
    d_spriteRightSide.setPosition({ backEnd, yOffset });

    updateSliderPosition();
  }

  void Slider::updateSliderPosition()
  {
    d_spriteSlider.setPosition({ d_spriteLeftSide.getSize().x + getSliderOffset(), 0 });
  }

  int Slider::getSliderOffset() const
  {
    const double ratio = (d_currentValue - d_minValue) / (d_maxValue - d_minValue);
    return (int)(d_length * ratio) - d_spriteSlider.getSize().x / 2;
  }

  double Slider::getSliderRatio(int i_offset) const
  {
    const double ratio = (double)i_offset / (double)d_length;
    return Sdk::clamp(ratio, 0.0, 1.0);
  }

  void Slider::followMouse()
  {
    const int mouseX =
      Game::get().getInputDevice().getMousePosition().x -
      (int)getPositionAbsolute().x -
      d_spriteBack.getPosition().x;

    setRelativeValue(getSliderRatio(mouseX));
  }

} // ns Dx
