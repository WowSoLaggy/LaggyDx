#pragma once

#include "Animation.h"
#include "IMesh.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class IModel
  {
  public:
    virtual ~IModel() = default;

    virtual const std::vector<std::shared_ptr<IMesh>>& getMeshes() const = 0;
    virtual const AnimationsMap& getAnimations() const = 0;
    virtual const Aabb& getAabb() const = 0;
    virtual const IMesh& getAabbMesh() const = 0;
  };

} // ns Dx
