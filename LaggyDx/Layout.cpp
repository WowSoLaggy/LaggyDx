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
    const auto size = getSize();
    const float xCenter = size.x / 2.0f + getPosition().x;
    float yNext = size.y + getPosition().y - d_offsetFromBorder;

    const int childrenCount = (int)getChildren().size();
    for (int i = (int)getChildren().size() - 1; i >= 0; --i)
    {
      auto& ctrl = dynamic_cast<IControl&>(*getChildren()[i]);
      const float x = xCenter - ctrl.getSize().x / 2.0f;
      yNext = yNext - ctrl.getSize().y;
      ctrl.setPosition({ x, yNext });
      yNext -= d_offset;
    }
  }


  void Layout::setOffset(int i_offset)
  {
    d_offset = i_offset;
  }

  void Layout::setOffsetFromBorder(int i_offsetFromBorder)
  {
    d_offsetFromBorder = i_offsetFromBorder;
  }


  void Layout::processEvent(const Sdk::IEvent& i_event)
  {
    if (dynamic_cast<const ControlSizeChangedEvent*>(&i_event))
      onChildrenChanged();
  }

} // ns Dx
