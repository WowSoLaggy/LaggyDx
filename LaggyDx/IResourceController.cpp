#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


namespace Dx
{
  std::shared_ptr<IResourceController> IResourceController::create()
  {
    return std::make_shared<ResourceController>();
  }
} // ns Dx
