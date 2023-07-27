#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IShape2d
  {
  public:
    static std::unique_ptr<IShape2d> createCustom(
      const std::vector<Sdk::Vector2F>& i_verts,
      const std::vector<int>& i_inds);

    static std::unique_ptr<IShape2d> createCircle(float i_radius, int i_numPoints);
    static std::unique_ptr<IShape2d> createCircle(float i_radius, int i_numPoints,
                                                float i_startAngle, float i_endAngle);

    static std::unique_ptr<IShape2d> createRect(float i_width, float i_height);

  public:
    virtual ~IShape2d() = default;

    virtual const std::vector<VertexPositionColor>& getVerts() const = 0;
    virtual const std::vector<int>& getInds() const = 0;
  };

} // ns Dx
