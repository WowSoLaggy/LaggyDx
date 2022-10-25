#pragma once

#include "LaggyDxFwd.h"
#include "Layout.h"


namespace Dx
{
  class RadioGroup : public Layout
  {
  public:
    virtual void processEvent(const Sdk::IEvent& i_event) override;

    virtual void addChild(std::shared_ptr<TreeNode> i_child) override;
    virtual void removeChild(TreeNode& i_child) override;

  private:
    void onRadioButtonCheck(const RadioButton& i_radioButton);
    bool isAnyChecked() const;
  };

} // ns Dx
