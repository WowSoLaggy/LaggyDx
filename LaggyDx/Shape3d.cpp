#include "stdafx.h"
#include "Shape3d.h"


namespace Dx
{
  Shape3d::Shape3d(std::vector<VertexTypePosTexNorm> i_verts, std::vector<int> i_inds)
  {
    setVerts(std::move(i_verts));
    setInds(std::move(i_inds));
  }


  const std::vector<VertexTypePosTexNorm>& Shape3d::getVerts() const { return d_verts; }
  const std::vector<int>& Shape3d::getInds() const { return d_inds; }

  void Shape3d::setVerts(std::vector<VertexTypePosTexNorm> i_verts)
  {
    d_verts = std::move(i_verts);
    calculateAabb();
  }
  void Shape3d::setInds(std::vector<int> i_inds) { d_inds = std::move(i_inds); }

  const Aabb& Shape3d::getAabb() const { return d_aabb; }


  void Shape3d::calculateAabb()
  {
    float minX = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = -std::numeric_limits<float>::max();

    for (const auto& vert : d_verts)
    {
      minX = std::min(minX, vert.position.x);
      maxX = std::max(maxX, vert.position.x);
      minY = std::min(minY, vert.position.y);
      maxY = std::max(maxY, vert.position.y);
      minZ = std::min(minZ, vert.position.z);
      maxZ = std::max(maxZ, vert.position.z);
    }

    d_aabb = Aabb(minX, maxX, minY, maxY, minZ, maxZ);
  }

} // ns Dx
