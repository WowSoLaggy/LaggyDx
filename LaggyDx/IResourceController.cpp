#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


namespace Dx
{
  std::unique_ptr<IResourceController> IResourceController::create(fs::path i_resourcesFolder)
  {
    return std::make_unique<ResourceController>(std::move(i_resourcesFolder));
  }
} // ns Dx
