#pragma once

#include "LaggyDxFwd.h"


class IAnimationController
{
public:
  static std::shared_ptr<IAnimationController> getAnimationController(
    const IResourceController& i_resourceController, ResourceId i_resourceId);

public:
  virtual const std::wstring& getCurrentAnimationName() const = 0;

  virtual void setAnimation(std::wstring i_animationName) = 0;
  virtual void update(double i_dt) = 0;
};
