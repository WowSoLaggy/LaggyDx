#include "stdafx.h"
#include "Shape2d.h"

namespace Dx
{
  Shape2d::Shape2d(
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

  
  const std::vector<VertexPositionColor>& Shape2d::getVerts() const { return d_verts; }
  const std::vector<int>& Shape2d::getInds() const { return d_inds; }

} // ns Dx
