#include "stdafx.h"
#include "Shape.h"

namespace Dx
{
  Shape::Shape(
    const std::vector<Sdk::Vector2F>& i_verts,
    const std::vector<int>& i_inds)
  {
    d_verts.resize(i_verts.size());
    std::transform(i_verts.cbegin(), i_verts.cend(), d_verts.begin(),
                   [](const auto& i_vert) {
                     VertexPositionColor v;
                     v.position = { i_vert.x, i_vert.y, 0 };
                     v.color = { 1.0f, 1.0f, 1.0f, 1.0f };
                     return v;
                   });

    d_inds.resize(i_inds.size());
    std::copy(i_inds.cbegin(), i_inds.cend(), d_inds.begin());
  }


  const VertexPositionColor* Shape::getVerts() const
  {
    return d_verts.data();
  }

  std::size_t Shape::getVertsCount() const
  {
    return d_verts.size();
  }


  const std::uint16_t* Shape::getInds() const
  {
    return d_inds.data();
  }

  std::size_t Shape::getIndsCount() const
  {
    return d_inds.size();
  }

} // ns Dx
