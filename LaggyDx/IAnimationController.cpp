#include "stdafx.h"
#include "IAnimationController.h"

#include "AnimationController.h"


namespace Dx
{
  std::shared_ptr<IAnimationController> IAnimationController::getAnimationController(
    const IResourceController& i_resourceController, ResourceId i_resourceId)
  {
    return std::make_shared<AnimationController>(i_resourceController, i_resourceId);
  }

} // ns Dx
