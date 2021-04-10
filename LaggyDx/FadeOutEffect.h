#pragma once

#include "GuiEffect.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class FadeOutEffect : public GuiEffect
  {
  public:
    /// FadeOut effects starts after @i_delay seconds are expired and lasts for @i_duration seconds
    FadeOutEffect(double i_delay, double i_duration);

    void setOnFinishHandler(std::function<void()> i_handler);

    virtual void setControl(IControl& i_control) override;

    virtual bool isFinished() const override;

    virtual void update(double i_dt) override;

  private:
    std::function<void()> d_onFinishHandler;

    double d_delay = 0;
    double d_currentOpacity = 1;
    double d_duration = 1;
    double d_decreaseSpeed = 1; // Speed == 1 means opacity from current value to 0 in 1 second
    bool d_isFinished = false;
  };

} // ns Dx
