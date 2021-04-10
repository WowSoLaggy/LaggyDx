#include "stdafx.h"
#include "GuiEffect.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  void GuiEffect::setControl(IControl& i_control)
  {
    d_control = &i_control;
  }


  IControl* GuiEffect::tryGetControl()
  {
    return d_control;
  }

  const IControl* GuiEffect::tryGetControl() const
  {
    return d_control;
  }

  IControl& GuiEffect::getControl()
  {
    CONTRACT_ASSERT(d_control);
    return *d_control;
  }

  const IControl& GuiEffect::getControl() const
  {
    CONTRACT_ASSERT(d_control);
    return *d_control;
  }

} // ns Dx
