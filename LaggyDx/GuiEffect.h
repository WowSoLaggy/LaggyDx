#pragma once

#include "IGuiEffect.h"


namespace Dx
{
  class GuiEffect : public IGuiEffect
  {
  public:
    virtual void setControl(IControl& i_control) override;
    
    virtual IControl* tryGetControl() override;
    virtual const IControl* tryGetControl() const override;
    virtual IControl& getControl() override;
    virtual const IControl& getControl() const override;

  private:
    IControl* d_control = nullptr;
  };

} // Dx
