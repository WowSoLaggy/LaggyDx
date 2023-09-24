#include "stdafx.h"
#include "Slider.h"

#include "App.h"

#include <LaggySdk/Math.h>
#include <LaggySdk/StringUtils.h>


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
    const auto pos = getPositionAbsolute();

    i_renderer.setTranslation(pos);

    i_renderer.renderSprite(d_spriteLeftSide);
    i_renderer.renderSprite(d_spriteBack);
    i_renderer.renderSprite(d_spriteRightSide);
    i_renderer.renderSprite(d_spriteSlider);

    if (d_showLabelMin)
      d_textMin.render(i_renderer, pos + d_textMinPosition);
    if (d_showLabelMax)
      d_textMax.render(i_renderer, pos + d_textMaxPosition);
    if (d_showLabelCurrent)
      d_textCurrent.render(i_renderer, pos + d_textCurrentPosition);
  }


  Sdk::Vector2F Slider::getSize() const
  {
    auto size = Sdk::Vector2I{
      d_spriteLeftSide.getSize().x + d_spriteBack.getSize().x + d_spriteRightSide.getSize().x,
      d_spriteSlider.getSize().y }.getVector<float>();

    if (d_showLabelMin)
      size.y = std::max(size.y, getLabelMinPosition().y + d_textMin.getSize().y);
    if (d_showLabelMax)
      size.y = std::max(size.y, getLabelMaxPosition().y + d_textMax.getSize().y);
    if (d_showLabelCurrent)
      size.y = std::max(size.y, getLabelCurrentPosition().y + d_textCurrent.getSize().y);

    return size;
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

  bool Slider::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Left)
    {
      const auto mousePos = App::get().getInputDevice().getMousePosition() - getPositionAbsolute().getVector<int>();

      if (d_spriteSlider.getRect().containsPoint(mousePos))
      {
        d_isCurrentlyDragged = true;
        return true;
      }
      else if (d_spriteLeftSide.getRect().containsPoint(mousePos))
      {
        setCurrentValue(d_minValue);
        return true;
      }
      else if (d_spriteRightSide.getRect().containsPoint(mousePos))
      {
        setCurrentValue(d_maxValue);
        return true;
      }
      else if (d_spriteBack.getRect().containsPoint(mousePos))
      {
        d_isCurrentlyDragged = true;
        followMouse();
        return true;
      }
    }

    return Control::onMouseClick(i_key);
  }

  bool Slider::onMouseRelease(MouseKey i_key)
  {
    if (i_key == MouseKey::Left && d_isCurrentlyDragged)
    {
      d_isCurrentlyDragged = false;
      return true;
    }

    return false;
  }


  void Slider::setTextureBack(const std::string& i_textureName)
  {
    const auto& rc = App::get().getResourceController();
    d_spriteBack.setTexture(rc.getTexture(i_textureName));

    rearrange();
  }

  void Slider::setTextureSlider(const std::string& i_textureName)
  {
    const auto& rc = App::get().getResourceController();
    d_spriteSlider.setTexture(rc.getTexture(i_textureName));
    d_spriteSlider.resetSizeToTexture();

    rearrange();
  }

  void Slider::setTextureRightSide(const std::string& i_textureName)
  {
    const auto& rc = App::get().getResourceController();
    d_spriteRightSide.setTexture(rc.getTexture(i_textureName));
    d_spriteRightSide.resetSizeToTexture();

    rearrange();
  }

  void Slider::setTextureLeftSide(const std::string& i_textureName)
  {
    const auto& rc = App::get().getResourceController();
    d_spriteLeftSide.setTexture(rc.getTexture(i_textureName));
    d_spriteLeftSide.resetSizeToTexture();

    rearrange();
  }


  void Slider::setFont(const std::string& i_fontName)
  {
    d_textMin.setFont(i_fontName);
    d_textMax.setFont(i_fontName);
    d_textCurrent.setFont(i_fontName);
  }

  void Slider::setLabelsScale(float i_scale)
  {
    d_textMin.setScale(i_scale);
    d_textMax.setScale(i_scale);
    d_textCurrent.setScale(i_scale);
    updateLabelsPosition();
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
    updateLabelMinText();
  }

  void Slider::setMaxValue(const double i_maxValue)
  {
    d_maxValue = i_maxValue;

    if (d_minValue >= d_maxValue)
      d_minValue = d_maxValue - std::numeric_limits<double>::epsilon();

    updateSliderPosition();
    updateLabelMaxText();
  }

  void Slider::setCurrentValue(const double i_currentValue)
  {
    if (i_currentValue == d_currentValue)
      return;

    d_currentValue = Sdk::clamp(i_currentValue, d_minValue, d_maxValue);
    
    updateSliderPosition();
    updateLabelCurrentText();
    callOnValueChanged();
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

    updateLabelsPosition();
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
      App::get().getInputDevice().getMousePosition().x -
      (int)getPositionAbsolute().x -
      d_spriteBack.getPosition().x;

    setRelativeValue(getSliderRatio(mouseX));
  }


  bool Slider::getShowLabelMin() const { return d_showLabelMin; }
  bool Slider::getShowLabelMax() const { return d_showLabelMax; }
  bool Slider::getShowLabelCurrent() const { return d_showLabelCurrent; }

  void Slider::setShowLabelMin(const bool i_show) { d_showLabelMin = i_show; }
  void Slider::setShowLabelMax(const bool i_show) { d_showLabelMax = i_show; }
  void Slider::setShowLabelCurrent(const bool i_show) { d_showLabelCurrent = i_show; }
  void Slider::setShowAllLabels(const bool i_show)
  {
    setShowLabelMin(i_show);
    setShowLabelMax(i_show);
    setShowLabelCurrent(i_show);
  }
  

  void Slider::setLabelsPrecision(const int i_digits)
  {
    d_labelsPrecision = i_digits;
    updateLabelsText();
  }

  int Slider::getLabelsPrecision() const
  {
    return d_labelsPrecision;
  }


  void Slider::callOnValueChanged()
  {
    if (d_onValueChangedHandler)
      d_onValueChangedHandler(getCurrentValue());
  }

  void Slider::setOnValueChangedHandler(OnValueChangedHandler i_onValueChangedHandler)
  {
    d_onValueChangedHandler = std::move(i_onValueChangedHandler);
  }


  Sdk::Vector2I Slider::getLabelMinPosition() const
  {
    const int textSizeHalfX = (int)(d_textMin.getSize().x / 2);
    return { 0, d_spriteSlider.getSize().y };
  }

  Sdk::Vector2I Slider::getLabelMaxPosition() const
  {
    const int textSizeX = (int)d_textMax.getSize().x;
    const int rightSideEndX = d_spriteRightSide.getPosition().x + d_spriteRightSide.getSize().x;
    return { rightSideEndX - textSizeX, d_spriteSlider.getSize().y };
  }

  Sdk::Vector2I Slider::getLabelCurrentPosition() const
  {
    const int textSizeHalfX = (int)(d_textCurrent.getSize().x / 2);
    const int barCenterX = d_spriteBack.getPosition().x + d_spriteBack.getSize().x / 2;
    return { barCenterX - textSizeHalfX, d_spriteSlider.getSize().y };
  }


  void Slider::updateLabelMinPosition()
  {
    d_textMinPosition = getLabelMinPosition().getVector<float>();
  }

  void Slider::updateLabelMaxPosition()
  {
    d_textMaxPosition = getLabelMaxPosition().getVector<float>();
  }

  void Slider::updateLabelCurrentPosition()
  {
    d_textCurrentPosition = getLabelCurrentPosition().getVector<float>();
  }

  void Slider::updateLabelsPosition()
  {
    updateLabelMinPosition();
    updateLabelMaxPosition();
    updateLabelCurrentPosition();
  }


  void Slider::updateLabelMinText()
  {
    d_textMin.setText(getLabelString(d_minValue));
    updateLabelMinPosition();
  }

  void Slider::updateLabelMaxText()
  {
    d_textMax.setText(getLabelString(d_maxValue));
    updateLabelMaxPosition();
  }

  void Slider::updateLabelCurrentText()
  {
    d_textCurrent.setText(getLabelString(d_currentValue));
    updateLabelCurrentPosition();
  }

  void Slider::updateLabelsText()
  {
    updateLabelMinText();
    updateLabelMaxText();
    updateLabelCurrentText();
  }


  std::string Slider::getLabelString(double i_value) const
  {
    return Sdk::toString(i_value, d_labelsPrecision);
  }

} // ns Dx
