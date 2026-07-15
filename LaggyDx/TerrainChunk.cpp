#include "stdafx.h"
#include "TerrainChunk.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  TerrainChunk::TerrainChunk(
    const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice,
    const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells)
    : d_origin(i_origin)
    , d_cells(i_cells)
  {
    CONTRACT_EXPECT(i_cells.x > 0);
    CONTRACT_EXPECT(i_cells.y > 0);
    build(i_heightMap, i_renderDevice);
  }


  const TerrainMipMap& TerrainChunk::getMipMap(const int i_lod) const
  {
    CONTRACT_EXPECT(0 <= i_lod && i_lod < (int)d_mipMaps.size());
    return d_mipMaps[i_lod];
  }


  void TerrainChunk::build(const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice)
  {
    const int maxCells = std::max(d_cells.x, d_cells.y);

    // Halve the sampling stride each LOD until a step spans the whole chunk (2 points left).
    for (int step = 1; step < maxCells; step *= 2)
      d_mipMaps.emplace_back(i_heightMap, i_renderDevice, d_origin, d_cells, step);

    if (d_mipMaps.empty())
      d_mipMaps.emplace_back(i_heightMap, i_renderDevice, d_origin, d_cells, 1);
  }

} // ns Dx
