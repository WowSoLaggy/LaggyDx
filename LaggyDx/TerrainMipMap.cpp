#include "stdafx.h"
#include "TerrainMipMap.h"

#include "HeightMap.h"
#include "IObject3.h"
#include "ModelUtils.h"
#include "Shape3d.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  namespace
  {
    std::vector<VertexPos3NormText> buildVerts(
      const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_pointCount, const int i_step)
    {
      std::vector<VertexPos3NormText> verts(i_pointCount * i_pointCount);

      int ind = 0;
      for (int yi = 0; yi < i_pointCount; ++yi)
      {
        const int y = i_origin.y + yi * i_step;
        for (int xi = 0; xi < i_pointCount; ++xi)
        {
          const int x = i_origin.x + xi * i_step;

          VertexPos3NormText p;
          p.position = { (float)x, (float)i_heightMap.getHeight(x, y), (float)y };
          p.normal = { 0.0f, 1.0f, 0.0f };
          p.texture = { (float)x, (float)y };
          verts[ind++] = std::move(p);
        }
      }

      return verts;
    }

    std::vector<int> buildInds(const int i_pointCount)
    {
      std::vector<int> inds;
      inds.reserve((i_pointCount - 1) * (i_pointCount - 1) * 6);

      for (int y = 0; y < i_pointCount - 1; ++y)
      {
        for (int x = 0; x < i_pointCount - 1; ++x)
        {
          const int i00 = x + y * i_pointCount;
          const int i10 = (x + 1) + y * i_pointCount;
          const int i01 = x + (y + 1) * i_pointCount;
          const int i11 = (x + 1) + (y + 1) * i_pointCount;

          inds.push_back(i00); inds.push_back(i01); inds.push_back(i10);
          inds.push_back(i10); inds.push_back(i01); inds.push_back(i11);
        }
      }

      return inds;
    }

    void calculateNormals(
      std::vector<VertexPos3NormText>& io_verts, const std::vector<int>& i_inds)
    {
      for (auto& v : io_verts)
        v.normal = { 0.0f, 0.0f, 0.0f };

      for (size_t i = 0; i + 2 < i_inds.size(); i += 3)
      {
        auto& v0 = io_verts[i_inds[i]];
        auto& v1 = io_verts[i_inds[i + 1]];
        auto& v2 = io_verts[i_inds[i + 2]];

        const auto faceNormal = (v1.position - v0.position).cross(v2.position - v0.position);
        v0.normal += faceNormal;
        v1.normal += faceNormal;
        v2.normal += faceNormal;
      }

      for (auto& v : io_verts)
        v.normal.normalize();
    }

  } // anonym NS


  TerrainMipMap::TerrainMipMap(
    const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_sizeInCells, const int i_step)
  {
    build(i_heightMap, i_origin, i_sizeInCells, i_step);
  }


  void TerrainMipMap::build(
    const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_sizeInCells, const int i_step)
  {
    // Check that we generate a valid square mesh -
    // i_sizeInCells must be a power of 2, and i_step must evenly divide it.
    CONTRACT_EXPECT(i_step >= 1);
    CONTRACT_EXPECT(i_sizeInCells >= 1);
    CONTRACT_EXPECT((i_sizeInCells & (i_sizeInCells - 1)) == 0); // power of 2
    CONTRACT_EXPECT(i_sizeInCells % i_step == 0);

    // Regular stride-spaced grid, edge-inclusive - relies on i_step evenly dividing i_sizeInCells.
    const int pointCount = i_sizeInCells / i_step + 1;

    auto verts = buildVerts(i_heightMap, i_origin, pointCount, i_step);
    auto inds = buildInds(pointCount);
    calculateNormals(verts, inds);

    const Shape3d shape(std::move(verts), std::move(inds));
    d_object = createObjectFromShape(shape, true);
  }

} // ns Dx
