#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IAnimationController3d
  {
  public:
    static std::shared_ptr<IAnimationController3d> getAnimationController(const IMeshResourceCmo& i_model);

  public:
    virtual const std::wstring& getCurrentAnimationName() const = 0;

    virtual void setAnimation(std::wstring i_animationName) = 0;
    virtual void update(double i_dt) = 0;
  };

} // ns Dx
