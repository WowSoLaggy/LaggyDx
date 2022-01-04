#include "stdafx.h"
#include "Control.h"

#include "IGuiEffect.h"


namespace Dx
{
  void Control::setPosition(Sdk::Vector2F i_position) { d_position = std::move(i_position); }
  const Sdk::Vector2F& Control::getPosition() const { return d_position; }

  void Control::setSize(Sdk::Vector2F i_size) { d_size = std::move(i_size); }
  Sdk::Vector2F Control::getSize() const { return d_size; }

  Sdk::RectF Control::getRect() const
  {
    return Sdk::RectF(getPosition(), getPosition() + getSize());
  }

  void Control::setOpacity(const double i_opacity) { d_opacity = i_opacity; }
  double Control::getOpacity() const { return d_opacity; }


  void Control::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    for (const auto& child : getChildren())
      std::dynamic_pointer_cast<IControl>(child)->render(i_renderer, i_parentPos + getPosition());
  }

  void Control::update(double i_dt)
  {
    for (auto& child : getChildren())
      std::dynamic_pointer_cast<IControl>(child)->update(i_dt);

    for (auto& effect : d_effects)
      effect->update(i_dt);
  }


  void Control::onMouseMove()
  {
    for (auto& child : getChildren())
      std::dynamic_pointer_cast<IControl>(child)->onMouseMove();
  }

  void Control::onMouseClick(MouseKey i_key)
  {
    for (auto& child : getChildren())
      std::dynamic_pointer_cast<IControl>(child)->onMouseClick(i_key);
  }

  void Control::onMouseRelease(MouseKey i_key)
  {
    for (auto& child : getChildren())
      std::dynamic_pointer_cast<IControl>(child)->onMouseRelease(i_key);
  }


  void Control::addEffect(std::shared_ptr<IGuiEffect> i_effect)
  {
    i_effect->setControl(*this);
    d_effects.push_back(i_effect);
  }


} // Dx
