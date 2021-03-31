#pragma once

#include "IShape.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Shape : public IShape
  {
  public:
    Shape(
      const std::vector<Sdk::Vector2F>& i_verts,
      const std::vector<int>& i_inds);

    virtual const DirectX::VertexPositionColor* getVerts() const override;
    virtual std::size_t getVertsCount() const override;

    virtual const std::uint16_t* getInds() const override;
    virtual std::size_t getIndsCount() const override;

  private:
    std::vector<VertexPositionColor> d_verts;
    std::vector<std::uint16_t> d_inds;
  };

} // ns Dx
