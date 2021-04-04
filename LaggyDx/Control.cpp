#include "stdafx.h"
#include "Control.h"

namespace Dx
{
  IControl* Control::getParent() { return d_parent; }
  const IControl* Control::getParent() const { return d_parent; }

  std::vector<std::shared_ptr<IControl>>& Control::getChildren() { return d_children; }
  const std::vector<std::shared_ptr<IControl>>& Control::getChildren() const { return d_children; }

  void Control::setPosition(Sdk::Vector2F i_position) { d_position = std::move(i_position); }
  const Sdk::Vector2F& Control::getPosition() const { return d_position; }

  void Control::setSize(Sdk::Vector2F i_size) { d_size = std::move(i_size); }
  const Sdk::Vector2F& Control::getSize() const { return d_size; }


  void Control::setParent(IControl* i_parent)
  {
    if (d_parent == i_parent)
      return;
    if (auto* oldParent = std::exchange(d_parent, i_parent))
      oldParent->removeChild(*this);
    if (d_parent)
      d_parent->addChild(shared_from_this());
  }

  void Control::addChild(std::shared_ptr<IControl> i_child)
  {
    if (i_child->getParent() != this)
      i_child->setParent(this);
    else
      d_children.push_back(i_child);
  }

  void Control::removeChild(IControl& i_child)
  {
    if (i_child.getParent() == this)
      i_child.setParent(nullptr);
    else
    {
      const auto it = std::find_if(d_children.cbegin(), d_children.cend(),
                                   [&](const auto i_childPtr) { return i_childPtr.get() == &i_child; });
      if (it != d_children.cend())
        d_children.erase(it);
    }
  }

  void Control::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    for (const auto child : d_children)
      child->render(i_renderer, i_parentPos + getPosition());
  }

  void Control::update(double i_dt)
  {
    for (auto child : d_children)
      child->update(i_dt);
  }

} // Dx
