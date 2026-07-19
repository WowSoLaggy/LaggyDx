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
    // Height-field normal from central differences of the 4 neighbors, sampled at mesh resolution.
    // Neighbor coords are clamped to the map, so map borders get a one-sided difference.
    Sdk::Vector3F heightMapNormal(const HeightMap& i_heightMap, const int i_x, const int i_y, const int i_step)
    {
      const Sdk::Vector2I& size = i_heightMap.getSize();
      const auto sampleX = [&](const int i_c) { return std::clamp(i_c, 0, size.x - 1); };
      const auto sampleY = [&](const int i_c) { return std::clamp(i_c, 0, size.y - 1); };

      const float hL = (float)i_heightMap.getHeight(sampleX(i_x - i_step), i_y);
      const float hR = (float)i_heightMap.getHeight(sampleX(i_x + i_step), i_y);
      const float hD = (float)i_heightMap.getHeight(i_x, sampleY(i_y - i_step));
      const float hU = (float)i_heightMap.getHeight(i_x, sampleY(i_y + i_step));

      Sdk::Vector3F normal{ hL - hR, 2.0f * (float)i_step, hD - hU };
      normal.normalize();
      return normal;
    }

    std::vector<VertexPos3NormText> buildVerts(
      const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_pointCount, const int i_step)
    {
      std::vector<VertexPos3NormText> verts(i_pointCount * i_pointCount);

      const int xEnd = i_origin.x + i_pointCount * i_step;
      const int yEnd = i_origin.y + i_pointCount * i_step;

      int ind = 0;
      for (int y = i_origin.y; y < yEnd; y += i_step)
      {
        for (int x = i_origin.x; x < xEnd; x += i_step)
        {
          VertexPos3NormText p;
          p.position = { (float)x, (float)i_heightMap.getHeight(x, y), (float)y };
          p.normal = heightMapNormal(i_heightMap, x, y, i_step);
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

  } // anonym NS


  TerrainMipMap::TerrainMipMap(
    const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_sizeInCells, const int i_step)
    : d_origin(i_origin)
    , d_sizeInCells(i_sizeInCells)
    , d_step(i_step)
  {
    build(i_heightMap);
  }


  void TerrainMipMap::build(const HeightMap& i_heightMap)
  {
    // Check that we generate a valid square mesh -
    // d_sizeInCells must be a power of 2, and d_step must evenly divide it.
    CONTRACT_EXPECT(d_step >= 1);
    CONTRACT_EXPECT(d_sizeInCells >= 1);
    CONTRACT_EXPECT((d_sizeInCells & (d_sizeInCells - 1)) == 0); // power of 2
    CONTRACT_EXPECT(d_sizeInCells % d_step == 0);

    // Regular stride-spaced grid, edge-inclusive - relies on d_step evenly dividing d_sizeInCells.
    const int pointCount = d_sizeInCells / d_step + 1;

    auto verts = buildVerts(i_heightMap, d_origin, pointCount, d_step);
    auto inds = buildInds(pointCount);

    const Shape3d shape(std::move(verts), std::move(inds));
    d_object = createObjectFromShape(shape, true);
  }

} // ns Dx
