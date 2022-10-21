#pragma once

#include "Control.h"
#include "Sprite.h"


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

    void setLength(int i_length);

    void setMinValue(double i_minValue);
    void setMaxValue(double i_maxValue);
    void setCurrentValue(double i_currentValue);
    void setRelativeValue(double i_relativeValue);
    double getCurrentValue() const;

  private:
    Sprite d_spriteBack;
    Sprite d_spriteSlider;
    Sprite d_spriteRightSide;
    Sprite d_spriteLeftSide;

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
  };

} // ns Dx
