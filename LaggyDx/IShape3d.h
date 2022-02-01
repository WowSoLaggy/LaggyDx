#pragma once

#include "VertexTypes.h"


namespace Dx
{
  class IShape3d
  {
  public:
    virtual ~IShape3d() = default;

    virtual const std::vector<VertexTypePosTexNorm>& getVerts() const = 0;
    virtual const std::vector<int>& getInds() const = 0;
    virtual const Aabb& getAabb() const = 0;
  };

} // ns Dx
