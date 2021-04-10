#pragma once

#include "IControl.h"


namespace Dx
{
  class Control : public IControl
  {
  public:
    virtual void setPosition(Sdk::Vector2F i_position) override;
    virtual const Sdk::Vector2F& getPosition() const override;

    virtual void setSize(Sdk::Vector2F i_size) override;
    virtual const Sdk::Vector2F& getSize() const override;

    virtual void setOpacity(double i_opacity) override;
    virtual double getOpacity() const override;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;
    virtual void update(double i_dt) override;

    virtual void addEffect(std::shared_ptr<IGuiEffect> i_effect) override;

  private:
    Sdk::Vector2F d_position;
    Sdk::Vector2F d_size;

    double d_opacity = 1;

    std::vector<std::shared_ptr<IGuiEffect>> d_effects;
  };

} // Dx
