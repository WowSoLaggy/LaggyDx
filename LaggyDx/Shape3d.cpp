#include "stdafx.h"
#include "Shape3d.h"


namespace Dx
{
  namespace
  {
    std::vector<VertexTypePosTexNorm> generatePlaneVerts(const Sdk::Size2I& i_size, const float i_dist)
    {
      std::vector<VertexTypePosTexNorm> verts(i_size.x * i_size.y);

      int ind = 0;
      for (int y = 0; y < i_size.y; ++y)
      {
        for (int x = 0; x < i_size.x; ++x)
        {
          VertexTypePosTexNorm p;
          p.position = { (float)x * i_dist, 0.0f, (float)y * i_dist };
          p.texture = { (float)x * i_dist, (float)y * i_dist };
          p.normal = { 0.0f, 1.0f, 0.0f };

          verts[ind++] = std::move(p);
        }
      }

      return verts;
    }

    std::vector<int> generatePlaneInds(const Sdk::Size2I& i_size)
    {
      //   0 1 2 3
      // 0 . . . .
      // 1 . . . .
      // 2 . . . .
      // 3 . . . .

      const int IndsCount = (i_size.x - 1) * (i_size.y - 1) * 3 * 2;
      std::vector<int> inds(IndsCount);

      int ind = 0;
      for (int y = 0; y < i_size.y - 1; ++y)
      {
        for (int x = 0; x < i_size.x - 1; ++x)
        {
          inds[ind++] = x + y * i_size.x;
          inds[ind++] = x + (y + 1) * i_size.x;
          inds[ind++] = (x + 1) + y * i_size.x;

          inds[ind++] = (x + 1) + y * i_size.x;
          inds[ind++] = x + (y + 1) * i_size.x;
          inds[ind++] = (x + 1) + (y + 1) * i_size.x;
        }
      }

      return inds;
    }

  } // anonym NS


  Shape3d Shape3d::plane(const Sdk::Size2I& i_size, const float i_dist)
  {
    auto verts = generatePlaneVerts(i_size, i_dist);
    auto inds = generatePlaneInds(i_size);
    return Shape3d(std::move(verts), std::move(inds));
  }


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
