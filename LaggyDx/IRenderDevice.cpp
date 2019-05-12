#include "stdafx.h"
#include "IRenderDevice.h"

#include "RenderDevice.h"


namespace Dx
{
  std::shared_ptr<IRenderDevice> IRenderDevice::create()
  {
    return std::make_shared<RenderDevice>();
  }

} // ns Dx
