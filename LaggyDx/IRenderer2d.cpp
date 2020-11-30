#include "stdafx.h"
#include "IRenderer2d.h"

#include "Renderer2d.h"


namespace Dx
{
  std::unique_ptr<IRenderer2d> IRenderer2d::create(IRenderDevice& io_renderDevice, const Sdk::Vector2I i_resolution)
  {
    return std::make_unique<Renderer2d>(io_renderDevice, std::move(i_resolution));
  }

} // ns Dx
