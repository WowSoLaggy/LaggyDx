#pragma once

#include "LaggyDxFwd.h"
#include "VertexTypes.h"

#include <LaggySdk/Size.h>


namespace Dx
{
  class IShape3d
  {
  public:
    /// \param i_size - number of points in each dimension (not real world size!)
    /// \param i_dist - distance between two adjacent points
    /// \param i_textureCoef - additional multiplier of texture coords
    static std::unique_ptr<IShape3d> plane(
      const Sdk::Size2I& i_size, float i_dist = 1.0f, float i_textureCoef = 1.0f);

    static std::unique_ptr<IShape3d> cube(float i_size);

  public:
    virtual ~IShape3d() = default;

    virtual const std::vector<VertexTypePosTexNorm>& getVerts() const = 0;
    virtual const std::vector<int>& getInds() const = 0;
    virtual const Aabb& getAabb() const = 0;
  };

} // ns Dx
