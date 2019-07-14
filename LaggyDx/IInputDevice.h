#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/SdkFwd.h>


namespace Dx
{
  class IInputDevice
  {
  public:

    static std::shared_ptr<IInputDevice> create();

  public:

    virtual ~IInputDevice() = default;

    virtual void initialize() = 0;
    virtual void dispose() = 0;

    virtual void processMessage(const Sdk::Message& i_inputMessage) = 0;

    virtual const MouseState& checkMouse() = 0;
    virtual const KeyboardState& checkKeyboard() = 0;

  };

} // ns Dx
