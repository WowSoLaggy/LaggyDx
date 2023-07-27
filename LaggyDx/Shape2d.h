#pragma once

#include "Colors.h"
#include "IShape2d.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Shape2d : public IShape2d
  {
  public:
    Shape2d(
      const std::vector<Sdk::Vector2F>& i_verts,
      const std::vector<int>& i_inds);

    virtual const std::vector<VertexPositionColor>& getVerts() const override;
    virtual const std::vector<int>& getInds() const override;

  private:
    std::vector<VertexPositionColor> d_verts;
    std::vector<int> d_inds;
  };

} // ns Dx
