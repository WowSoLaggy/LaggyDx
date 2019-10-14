#include "stdafx.h"
#include "IRenderer3d.h"

#include "Renderer3d.h"


namespace Dx
{
  std::shared_ptr<IRenderer3d> IRenderer3d::create(
    IRenderDevice& io_renderDevice,
    IResourceController& i_resourceController,
    const ICamera& i_camera)
  {
    return std::make_shared<Renderer3d>(io_renderDevice, i_resourceController, i_camera);
  }

} // ns Dx
