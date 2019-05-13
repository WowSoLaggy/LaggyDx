#include "stdafx.h"
#include "IInputDevice.h"

#include "InputDevice.h"


namespace Dx
{
  std::shared_ptr<IInputDevice> IInputDevice::create()
  {
    return std::make_shared<InputDevice>();
  }

} // ns Dx
