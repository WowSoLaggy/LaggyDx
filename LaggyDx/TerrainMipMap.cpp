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
    // Grid point coords of this LOD along one axis - stride-spaced, last point pinned to the chunk edge.
    std::vector<int> axisCoords(const int i_origin, const int i_cells, const int i_step)
    {
      std::vector<int> coords;
      for (int c = 0; c < i_cells; c += i_step)
        coords.push_back(i_origin + c);
      coords.push_back(i_origin + i_cells);
      return coords;
    }

    std::vector<VertexPos3NormText> buildVerts(
      const HeightMap& i_heightMap,
      const std::vector<int>& i_xs, const std::vector<int>& i_ys)
    {
      std::vector<VertexPos3NormText> verts(i_xs.size() * i_ys.size());

      int ind = 0;
      for (const int y : i_ys)
      {
        for (const int x : i_xs)
        {
          VertexPos3NormText p;
          p.position = { (float)x, (float)i_heightMap.getHeight(x, y), (float)y };
          p.normal = { 0.0f, 1.0f, 0.0f };
          p.texture = { (float)x, (float)y };
          verts[ind++] = std::move(p);
        }
      }

      return verts;
    }

    std::vector<int> buildInds(const int i_xCount, const int i_yCount)
    {
      std::vector<int> inds;
      inds.reserve((i_xCount - 1) * (i_yCount - 1) * 6);

      for (int y = 0; y < i_yCount - 1; ++y)
      {
        for (int x = 0; x < i_xCount - 1; ++x)
        {
          const int i00 = x + y * i_xCount;
          const int i10 = (x + 1) + y * i_xCount;
          const int i01 = x + (y + 1) * i_xCount;
          const int i11 = (x + 1) + (y + 1) * i_xCount;

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
    const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice,
    const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells, const int i_step)
    : d_step(i_step)
  {
    CONTRACT_EXPECT(i_step >= 1);
    build(i_heightMap, i_renderDevice, i_origin, i_cells);
  }


  void TerrainMipMap::build(
    const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice,
    const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells)
  {
    const auto xs = axisCoords(i_origin.x, i_cells.x, d_step);
    const auto ys = axisCoords(i_origin.y, i_cells.y, d_step);

    auto verts = buildVerts(i_heightMap, xs, ys);
    auto inds = buildInds((int)xs.size(), (int)ys.size());
    calculateNormals(verts, inds);

    const Shape3d shape(std::move(verts), std::move(inds));
    d_object = createObjectFromShape(shape, i_renderDevice, true);
  }

} // ns Dx
