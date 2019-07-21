#include "stdafx.h"
#include "IAnimationController.h"

#include "AnimationController.h"


namespace Dx
{
  std::shared_ptr<IAnimationController> IAnimationController::getAnimationController(const IMeshResourceCmo& i_model)
  {
    return std::make_shared<AnimationController>(i_model);
  }

} // ns Dx
