#include "stdafx.h"
#include "Control.h"

#include "IGuiEffect.h"


namespace Dx
{
  void Control::setPosition(Sdk::Vector2F i_position) { d_position = std::move(i_position); }
  const Sdk::Vector2F& Control::getPosition() const { return d_position; }

  void Control::setSize(Sdk::Vector2F i_size) { d_size = std::move(i_size); }
  const Sdk::Vector2F& Control::getSize() const { return d_size; }

  void Control::setOpacity(const double i_opacity) { d_opacity = i_opacity; }
  double Control::getOpacity() const { return d_opacity; }


  void Control::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    for (const auto& child : getChildren())
      std::dynamic_pointer_cast<Control>(child)->render(i_renderer, i_parentPos + getPosition());
  }

  void Control::update(double i_dt)
  {
    for (auto& child : getChildren())
      std::dynamic_pointer_cast<Control>(child)->update(i_dt);

    for (auto& effect : d_effects)
      effect->update(i_dt);
  }


  void Control::addEffect(std::shared_ptr<IGuiEffect> i_effect)
  {
    i_effect->setControl(*this);
    d_effects.push_back(i_effect);
  }


} // Dx
