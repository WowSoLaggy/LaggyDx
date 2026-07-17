#include "stdafx.h"
#include "TerrainChunk.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  TerrainChunk::TerrainChunk(
    const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_cells)
    : d_origin(i_origin)
    , d_cells(i_cells)
  {
    build(i_heightMap);
  }


  const TerrainMipMap& TerrainChunk::getMipMap(const int i_lod) const
  {
    CONTRACT_EXPECT(0 <= i_lod && i_lod < (int)d_mipMaps.size());
    return d_mipMaps[i_lod];
  }


  void TerrainChunk::build(const HeightMap& i_heightMap)
  {
    CONTRACT_EXPECT(d_cells > 0);

    // Halve the sampling stride each LOD until a step spans the whole chunk (2 points left).
    for (int step = 1; step < d_cells; step *= 2)
      d_mipMaps.emplace_back(i_heightMap, d_origin, d_cells, step);

    if (d_mipMaps.empty())
      d_mipMaps.emplace_back(i_heightMap, d_origin, d_cells, 1);
  }

} // ns Dx
