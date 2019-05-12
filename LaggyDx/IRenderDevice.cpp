#include "stdafx.h"
#include "IRenderDevice.h"

#include "RenderDevice.h"


std::shared_ptr<IRenderDevice> IRenderDevice::create()
{
  return std::make_shared<RenderDevice>();
}
