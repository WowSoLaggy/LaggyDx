#include "stdafx.h"
#include "TerrainChunk.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  TerrainChunk::TerrainChunk(
    const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const int i_cells, const int i_lodsCount)
    : d_origin(i_origin)
    , d_cells(i_cells)
  {
    build(i_heightMap, i_lodsCount);
  }


  const TerrainMipMap& TerrainChunk::getMipMap(const int i_lod) const
  {
    CONTRACT_EXPECT(0 <= i_lod && i_lod < (int)d_mipMaps.size());
    return d_mipMaps[i_lod];
  }


  void TerrainChunk::build(const HeightMap& i_heightMap, const int i_lodsCount)
  {
    // d_cells >= 2 guarantees the loop runs at least once (step 1 always fits).
    CONTRACT_EXPECT(d_cells >= 2);
    CONTRACT_EXPECT(i_lodsCount > 0);

    // Halve the sampling stride each LOD until a step spans the whole chunk or we hit the LOD cap.
    for (int step = 1; step < d_cells && (int)d_mipMaps.size() < i_lodsCount; step *= 2)
      d_mipMaps.emplace_back(i_heightMap, d_origin, d_cells, step);

    CONTRACT_ENSURE(!d_mipMaps.empty());
  }

} // ns Dx
