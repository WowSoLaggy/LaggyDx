#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/IEvent.h>


namespace Dx
{
  class ControlSizeChangedEvent : public Sdk::IEvent
  {
  };

  class ControlVisibilityChangedEvent : public Sdk::IEvent
  {
  };


  class RadioButtonCheckEvent : public Sdk::IEvent
  {
  public:
    RadioButtonCheckEvent(const RadioButton& i_radioButton)
      : d_radioButton(i_radioButton)
    {
    }

    const RadioButton& getControl() const { return d_radioButton; }

  private:
    const RadioButton& d_radioButton;
  };


  class ButtonPressedEvent : public Sdk::IEvent
  {
  };

} // ns Dx
