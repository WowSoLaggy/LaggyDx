#pragma once

#include "Animation.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class IModel
  {
  public:
    virtual ~IModel() = default;

    virtual const std::vector<Mesh>& getMeshes() const = 0;
    virtual const AnimationsMap& getAnimations() const = 0;
  };

} // ns Dx
