#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IShape
  {
  public:
    static std::shared_ptr<IShape> createCustom(
      const std::vector<Sdk::Vector2F>& i_verts,
      const std::vector<int>& i_inds);

    static std::shared_ptr<IShape> createCircle(float i_radius, int i_numPoints);

  public:
    virtual ~IShape() = default;

    virtual const DirectX::VertexPositionColor* getVerts() const = 0;
    virtual std::size_t getVertsCount() const = 0;

    virtual const std::uint16_t* getInds() const = 0;
    virtual std::size_t getIndsCount() const = 0;
  };

} // ns Dx
