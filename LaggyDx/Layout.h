#pragma once

#include "Control.h"


namespace Dx
{
  enum class LayoutAlign
  {
    TopToBottom_LeftSide, // controls are aligned from top to bottom, along the left layout border
    TopToBottom_Center, // controls are aligned from top to bottom, along the center, start from the center
    TopToBottom_Center_FromTop, // controls are aligned from top to bottom, along the center, start from the top
    TopToBottom_CenterBottom, // controls are aligned from top to bottom, from the center bottom
    LeftToRight_TopSide,  // controls are aligned from left to right, along the top layout border
    LeftToRight_BottomSide,  // controls are aligned from left to right, along the bottom layout border
    RightToLeft_BottomSide,  // controls are aligned along the bottom layout border, sticking to the right border
  };

  class Layout : public Control
  {
  public:
    virtual void processEvent(const Sdk::IEvent& i_event) override;

    virtual void addChild(std::shared_ptr<TreeNode> i_child) override;
    virtual void removeChild(TreeNode& i_child) override;
    virtual void removeChildren() override;

    virtual bool getIsClickable() const override;

    void setOffsetBetweenElements(int i_offset);
    int getOffsetBetweenElements() const;

    void setOffsetFromBorder(int i_offset);
    int getOffsetFromBorder() const;

    void setAlign(LayoutAlign i_align);
    LayoutAlign getAlign() const;

    void setDynamicSizeX(bool i_dynamic);
    void setDynamicSizeY(bool i_dynamic);
    bool getDynamicSizeX() const;
    bool getDynamicSizeY() const;

  private:
    int d_offsetBetweenElements = 0;
    int d_offsetFromBorder = 0;

    LayoutAlign d_align = LayoutAlign::TopToBottom_LeftSide;

    bool d_dynamicSizeX = false;
    bool d_dynamicSizeY = false;
    void calculateDynamicSize();

    void onChildrenChanged();
    void rearrange();
  };

} // ns Dx
