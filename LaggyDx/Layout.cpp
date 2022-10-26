#include "stdafx.h"
#include "Layout.h"

#include "ControlEvents.h"


namespace Dx
{
  void Layout::processEvent(const Sdk::IEvent& i_event)
  {
    if (dynamic_cast<const ControlSizeChangedEvent*>(&i_event))
      onChildrenChanged();
    else if (dynamic_cast<const ControlVisibilityChangedEvent*>(&i_event))
      onChildrenChanged();
  }


  void Layout::addChild(std::shared_ptr<Sdk::TreeNode> i_child)
  {
    auto* childControl = dynamic_cast<IControl*>(i_child.get());
    CONTRACT_ASSERT(childControl);
    connectTo(*childControl);

    Control::addChild(i_child);
    onChildrenChanged();
  }

  void Layout::removeChild(Sdk::TreeNode& i_child)
  {
    auto* childControl = dynamic_cast<IControl*>(&i_child);
    CONTRACT_ASSERT(childControl);
    disconnectFrom(*childControl);

    Control::removeChild(i_child);
    onChildrenChanged();
  }

  void Layout::removeChildren()
  {
    Control::removeChildren();
    onChildrenChanged();
  }

  
  void Layout::onChildrenChanged()
  {
    rearrange();
  }

  void Layout::rearrange()
  {
    const auto bordersOffset = Sdk::Vector2F{ (float)d_offsetFromBorder, (float)d_offsetFromBorder };

    switch (d_align)
    {
    case LayoutAlign::TopToBottom_LeftSide:
    {
      float yNext = 0;
      for (auto& ctrlNode : getChildren())
      {
        auto& ctrl = dynamic_cast<IControl&>(*ctrlNode);
        if (!ctrl.getVisible())
          continue;

        const auto newPos = bordersOffset + Sdk::Vector2F{ 0, yNext };
        ctrl.setPosition(newPos);

        yNext += ctrl.getSize().y + d_offsetBetweenElements;
      }

      break;
    }
    case LayoutAlign::LeftToRight_TopSide:
    {
      float xNext = 0;
      for (auto& ctrlNode : getChildren())
      {
        auto& ctrl = dynamic_cast<IControl&>(*ctrlNode);
        if (!ctrl.getVisible())
          continue;

        const auto newPos = bordersOffset + Sdk::Vector2F{ xNext, 0 };
        ctrl.setPosition(newPos);

        xNext += ctrl.getSize().x + d_offsetBetweenElements;
      }

      break;
    }
    default:
      CONTRACT_ASSERT(false);
    }

    if (getDynamicSizeX() || getDynamicSizeY())
      calculateDynamicSize();
  }


  void Layout::setOffsetBetweenElements(const int i_offset)
  {
    d_offsetBetweenElements = i_offset;
    rearrange();
  }

  int Layout::getOffsetBetweenElements() const
  {
    return d_offsetBetweenElements;
  }

  void Layout::setOffsetFromBorder(const int i_offset)
  {
    d_offsetFromBorder = i_offset;
    rearrange();
  }

  int Layout::getOffsetFromBorder() const
  {
    return d_offsetFromBorder;
  }

  void Layout::setAlign(LayoutAlign i_align)
  {
    d_align = i_align;
    rearrange();
  }

  LayoutAlign Layout::getAlign() const
  {
    return d_align;
  }


  void Layout::setDynamicSizeX(bool i_dynamic)
  {
    d_dynamicSizeX = i_dynamic;
  }

  void Layout::setDynamicSizeY(bool i_dynamic)
  {
    d_dynamicSizeY = i_dynamic;
  }

  bool Layout::getDynamicSizeX() const
  {
    return d_dynamicSizeX;
  }

  bool Layout::getDynamicSizeY() const
  {
    return d_dynamicSizeY;
  }


  void Layout::calculateDynamicSize()
  {
    Sdk::RectF rect;
    for (auto& ctrlNode : getChildren())
    {
      auto& ctrl = dynamic_cast<IControl&>(*ctrlNode);
      if (ctrl.getVisible())
        rect = addRects(rect, ctrl.getRectRelative());
    }

    rect.expand(getOffsetFromBorder());
    
    auto size = rect.size();
    if (!getDynamicSizeX())
      size.x = getSize().x;
    if (!getDynamicSizeY())
      size.y = getSize().y;

    setSize(size);
  }

} // ns Dx
