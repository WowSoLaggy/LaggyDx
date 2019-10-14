#include "stdafx.h"
#include "IRenderer2d.h"

#include "Renderer2d.h"


namespace Dx
{
  std::unique_ptr<IRenderer2d> IRenderer2d::create(IRenderDevice& io_renderDevice)
  {
    return std::make_unique<Renderer2d>(io_renderDevice);
  }

} // ns Dx
