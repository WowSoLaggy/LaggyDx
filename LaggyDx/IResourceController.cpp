#include "stdafx.h"
#include "IResourceController.h"

#include "ResourceController.h"


std::shared_ptr<IResourceController> IResourceController::create()
{
  return std::make_shared<ResourceController>();
}