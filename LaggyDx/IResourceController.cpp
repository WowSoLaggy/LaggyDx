#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


namespace Dx
{
  std::unique_ptr<IResourceController> IResourceController::create(const std::string& i_resourcesFolder)
  {
    return std::make_unique<ResourceController>(i_resourcesFolder);
  }
} // ns Dx
