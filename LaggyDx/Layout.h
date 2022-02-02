#pragma once

#include "Control.h"


namespace Dx
{
  class Layout : public Control
  {
  public:
    virtual void addChild(std::shared_ptr<TreeNode> i_child) override;
    virtual void removeChild(TreeNode& i_child) override;
    virtual void removeChildren() override;

    void setOffset(int i_offset);
    void setOffsetFromBorder(int i_offsetFromBorder);

    virtual void processEvent(const Sdk::IEvent& i_event) override;

  private:
    int d_offset = 0;
    int d_offsetFromBorder = 0;

    void onChildrenChanged();
  };

} // ns Dx
