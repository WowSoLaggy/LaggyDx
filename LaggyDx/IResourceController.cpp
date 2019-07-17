#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


namespace Dx
{
  std::unique_ptr<IResourceController> IResourceController::create()
  {
    return std::make_unique<ResourceController>();
  }
} // ns Dx
