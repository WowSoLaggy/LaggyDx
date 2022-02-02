#pragma once

#include "IControl.h"


namespace Dx
{
  class Control : public IControl
  {
  public:
    virtual void setPosition(Sdk::Vector2F i_position) override;
    virtual Sdk::Vector2F getPosition() const override;

    virtual void setSize(Sdk::Vector2F i_size) override;
    virtual Sdk::Vector2F getSize() const override;

    virtual Sdk::RectF getRect() const override;

    virtual void setOpacity(double i_opacity) override;
    virtual double getOpacity() const override;

    virtual void setVisible(bool i_visible) override;
    virtual bool getVisible() const override;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;
    virtual void update(double i_dt) override;

    virtual void onMouseMove() override;
    virtual void onMouseClick(MouseKey i_key) override;
    virtual void onMouseRelease(MouseKey i_key) override;

    virtual void addEffect(std::shared_ptr<IGuiEffect> i_effect) override;

  private:
    Sdk::Vector2F d_position;
    Sdk::Vector2F d_size;

    bool d_visible = true;
    double d_opacity = 1;

    std::vector<std::shared_ptr<IGuiEffect>> d_effects;
  };

} // Dx
