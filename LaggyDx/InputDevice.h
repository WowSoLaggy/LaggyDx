#pragma once

#include "IInputDevice.h"
#include "KeyboardState.h"


namespace Dx
{
  class InputDevice : public IInputDevice
  {
  public:

    virtual void initialize() override;
    virtual void dispose() override;

    virtual void processMessage(const Sdk::Message& i_inputMessage) override;

    virtual const KeyboardState& check() override;

  private:

    std::unique_ptr<Keyboard> d_keyboard;
    KeyboardState d_keyboardState;

  };

} // ns Dx
