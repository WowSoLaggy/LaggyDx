#include "stdafx.h"
#include "Layout.h"

#include "ControlEvents.h"


namespace Dx
{
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

        const auto newPos = bordersOffset + Sdk::Vector2F{ xNext, 0 };
        ctrl.setPosition(newPos);

        xNext += ctrl.getSize().x + d_offsetBetweenElements;
      }

      break;
    }
    default:
      CONTRACT_ASSERT(false);
    }
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


  void Layout::processEvent(const Sdk::IEvent& i_event)
  {
    if (dynamic_cast<const ControlSizeChangedEvent*>(&i_event))
      onChildrenChanged();
  }

} // ns Dx
