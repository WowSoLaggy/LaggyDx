#include "stdafx.h"
#include "IAnimationController3d.h"

#include "AnimationController3d.h"


namespace Dx
{
  std::shared_ptr<IAnimationController3d> IAnimationController3d::getAnimationController(const IMeshResourceCmo& i_model)
  {
    return std::make_shared<AnimationController3d>(i_model);
  }

} // ns Dx
