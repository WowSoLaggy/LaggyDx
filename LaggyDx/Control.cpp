#include "stdafx.h"
#include "Control.h"

#include "App.h"
#include "ControlEvents.h"
#include "IGuiEffect.h"


namespace Dx
{
  void Control::setPosition(Sdk::Vector2F i_position) { d_position = std::move(i_position); }
  Sdk::Vector2F Control::getPositionAbsolute() const
  {
    if (const auto* parentControl = dynamic_cast<const Control*>(getParent()))
      return d_position + parentControl->getPositionAbsolute();
    else
      return d_position;
  }
  Sdk::Vector2F Control::getPositionRelative() const
  {
      return d_position;
  }

  void Control::setSize(Sdk::Vector2F i_size)
  {
    d_size = std::move(i_size);
    notify(ControlSizeChangedEvent());
  }
  Sdk::Vector2F Control::getSize() const { return d_size; }

  Sdk::RectF Control::getRectAbsolute() const
  {
    return Sdk::RectF(getPositionAbsolute(), getPositionAbsolute() + getSize());
  }
  Sdk::RectF Control::getRectRelative() const
  {
    return Sdk::RectF(getPositionRelative(), getPositionRelative() + getSize());
  }

  void Control::setOpacity(const double i_opacity) { d_opacity = i_opacity; }
  double Control::getOpacity() const { return d_opacity; }

  void Control::setVisible(bool i_visible)
  {
    d_visible = i_visible;
    notify(ControlVisibilityChangedEvent());
  }
  bool Control::getVisible() const { return d_visible; }


  void Control::render(IRenderer2d& i_renderer) const
  {
    for (const auto& child : getChildren())
    {
      const auto childPtr = std::dynamic_pointer_cast<IControl>(child);
      if (childPtr->getVisible())
        childPtr->render(i_renderer);
    }
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
    for (auto& child : std::ranges::reverse_view(getChildren()))
    {
      const auto childPtr = std::dynamic_pointer_cast<IControl>(child);
      if (childPtr->getVisible())
        childPtr->onMouseMove();
    }
  }

  bool Control::onMouseClick(MouseKey i_key)
  {
    if (!getVisible())
      return false;

    for (auto& child : std::ranges::reverse_view(getChildren()))
    {
      const auto childPtr = std::dynamic_pointer_cast<IControl>(child);
      if (childPtr->getVisible())
      {
        if (childPtr->onMouseClick(i_key))
          return true;
      }
    }

    const auto& mousePos = App::get().getInputDevice().getMousePosition();
    return getRectAbsolute().containsPoint(mousePos.getVector<float>());
  }

  bool Control::onMouseRelease(MouseKey i_key)
  {
    for (auto& child : std::ranges::reverse_view(getChildren()))
    {
      const auto childPtr = std::dynamic_pointer_cast<IControl>(child);
      if (childPtr->getVisible())
      {
        if (childPtr->onMouseRelease(i_key))
          return true;
      }
    }

    return false;
  }


  void Control::addEffect(std::shared_ptr<IGuiEffect> i_effect)
  {
    i_effect->setControl(*this);
    d_effects.push_back(i_effect);
  }

} // Dx
