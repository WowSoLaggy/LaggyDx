#include "stdafx.h"
#include "FadeOutEffect.h"

#include "IControl.h"


namespace Dx
{
  FadeOutEffect::FadeOutEffect(double i_delay, const double i_duration)
  {
    d_delay = i_delay;
    d_duration = i_duration;
  }

  void FadeOutEffect::setControl(IControl& i_control)
  {
    d_decreaseSpeed = i_control.getOpacity() / d_duration;
    GuiEffect::setControl(i_control);
  }

  void FadeOutEffect::setOnFinishHandler(std::function<void()> i_handler) { d_onFinishHandler = i_handler; }
  bool FadeOutEffect::isFinished() const { return d_isFinished; }

  void FadeOutEffect::update(double i_dt)
  {
    if (d_isFinished)
      return;

    if (d_delay > 0)
    {
      d_delay -= i_dt;
      return;
    }

    const double newOpacity = getControl().getOpacity() - d_decreaseSpeed * i_dt;

    if (newOpacity < 0)
    {
      getControl().setOpacity(0);
      d_isFinished = true;
      if (d_onFinishHandler)
        d_onFinishHandler();
    }
    else
      getControl().setOpacity(newOpacity);
  }

} // ns Dx
