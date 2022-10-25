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
    if (d_align != LayoutAlign::TopToBottom_LeftSide)
    {
      // Sorry, we don't support anything else so far
      CONTRACT_ASSERT(false);
    }

    float yNext = 0;
    for (int i = 0; i < (int)getChildren().size(); ++i)
    {
      auto& ctrl = dynamic_cast<IControl&>(*getChildren()[i]);
      ctrl.setPosition({ 0, yNext });
      yNext += ctrl.getSize().y + d_offsetBetweenElements;
    }

    const auto bordersOffset = Sdk::Vector2F{ (float)d_offsetFromBorder, (float)d_offsetFromBorder };
    for (int i = 0; i < (int)getChildren().size(); ++i)
    {
      auto& ctrl = dynamic_cast<IControl&>(*getChildren()[i]);
      ctrl.setPosition(ctrl.getPositionRelative() + bordersOffset);
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
