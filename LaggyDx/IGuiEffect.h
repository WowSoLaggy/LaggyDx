#pragma once

#include "LaggyDxFwd.h"

namespace Dx
{
  class IGuiEffect
  {
  public:
    virtual ~IGuiEffect() = default;

    virtual void setControl(IControl& i_control) = 0;
    
    virtual IControl* tryGetControl()= 0;
    virtual const IControl* tryGetControl() const= 0;
    virtual IControl& getControl()= 0;
    virtual const IControl& getControl() const= 0;

    virtual void update(double i_dt) = 0;

    virtual bool isFinished() const = 0;
  };

} // ns Dx
