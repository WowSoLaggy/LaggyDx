#pragma once

#include "Control.h"
#include "Sprite.h"
#include "Text.h"


namespace Dx
{
  class Slider : public Control
  {
  public:
    Slider();

    virtual void render(IRenderer2d& i_renderer) const override;

    virtual Sdk::Vector2F getSize() const override;
    /// Returns sum of sizes of both left and right sides
    Sdk::Vector2I getSidesSize() const;

    virtual void onMouseMove() override;
    virtual void onMouseClick(MouseKey i_key) override;
    virtual void onMouseRelease(MouseKey i_key) override;

    void setTextureBack(const std::string& i_textureName);
    void setTextureSlider(const std::string& i_textureName);
    void setTextureRightSide(const std::string& i_textureName);
    void setTextureLeftSide(const std::string& i_textureName);

    void setFont(const std::string& i_fontName);
    void setLabelsScale(float i_scale);

    void setLength(int i_length);

    void setMinValue(double i_minValue);
    void setMaxValue(double i_maxValue);
    void setCurrentValue(double i_currentValue);
    void setRelativeValue(double i_relativeValue);
    double getCurrentValue() const;

    bool getShowLabelMin() const;
    bool getShowLabelMax() const;
    bool getShowLabelCurrent() const;

    void setShowLabelMin(bool i_show);
    void setShowLabelMax(bool i_show);
    void setShowLabelCurrent(bool i_show);
    void setShowAllLabels(bool i_show);

    /// \param i_digits - number of digits to be shown after point
    void setLabelsPrecision(int i_digits);
    int getLabelsPrecision() const;

  private:
    Sprite d_spriteBack;
    Sprite d_spriteSlider;
    Sprite d_spriteRightSide;
    Sprite d_spriteLeftSide;

    Text d_textMin;
    Text d_textMax;
    Text d_textCurrent;
    Sdk::Vector2F d_textMinPosition;
    Sdk::Vector2F d_textMaxPosition;
    Sdk::Vector2F d_textCurrentPosition;

    bool d_showLabelMin = true;
    bool d_showLabelMax = true;
    bool d_showLabelCurrent = true;
    int d_labelsPrecision = 0;

    int d_length = 0;
    bool d_isCurrentlyDragged = false;

    double d_minValue = 0;
    double d_maxValue = 0;
    double d_currentValue = 0;

    void rearrange();
    void updateSliderPosition();
    int getSliderOffset() const;
    /// \param i_offset - int relatively to d_spriteBack position
    double getSliderRatio(int i_offset) const;
    void followMouse();

    Sdk::Vector2I getLabelMinPosition() const;
    Sdk::Vector2I getLabelMaxPosition() const;
    Sdk::Vector2I getLabelCurrentPosition() const;
    
    void updateLabelMinPosition();
    void updateLabelMaxPosition();
    void updateLabelCurrentPosition();
    void updateLabelsPosition();

    void updateLabelMinText();
    void updateLabelMaxText();
    void updateLabelCurrentText();
    void updateLabelsText();

    std::string getLabelString(double i_value) const;
  };

} // ns Dx
