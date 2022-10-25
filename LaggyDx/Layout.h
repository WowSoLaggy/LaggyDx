#pragma once

#include "Control.h"


namespace Dx
{
  enum class LayoutAlign
  {
    TopToBottom_LeftSide,
  };

  class Layout : public Control
  {
  public:
    virtual void addChild(std::shared_ptr<TreeNode> i_child) override;
    virtual void removeChild(TreeNode& i_child) override;
    virtual void removeChildren() override;

    void setOffsetBetweenElements(int i_offset);
    int getOffsetBetweenElements() const;

    void setOffsetFromBorder(int i_offset);
    int getOffsetFromBorder() const;

    void setAlign(LayoutAlign i_align);
    LayoutAlign getAlign() const;

    virtual void processEvent(const Sdk::IEvent& i_event) override;

  private:
    int d_offsetBetweenElements = 0;
    int d_offsetFromBorder = 0;

    LayoutAlign d_align = LayoutAlign::TopToBottom_LeftSide;

    void onChildrenChanged();
    void rearrange();
  };

} // ns Dx
