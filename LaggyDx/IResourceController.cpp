#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


namespace Dx
{
  std::unique_ptr<IResourceController> IResourceController::create(IRenderDevice& i_renderDevice,
                                                                   fs::path i_resourcesFolder)
  {
    return std::make_unique<ResourceController>(i_renderDevice, std::move(i_resourcesFolder));
  }
} // ns Dx
