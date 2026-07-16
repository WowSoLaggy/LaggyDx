#pragma once

#include "LaggyDxFwd.h"
#include "TerrainMipMap.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // One square tile of the terrain, holding a mesh per LOD (finest first).
  class TerrainChunk
  {
  public:
    // \param i_origin - chunk's top-left corner in height map grid coords
    // \param i_cells - chunk's size in grid cells
    TerrainChunk(
      const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells);

    const Sdk::Vector2I& getOrigin() const { return d_origin; }
    const Sdk::Vector2I& getCells() const { return d_cells; }

    int getMipMapsCount() const { return (int)d_mipMaps.size(); }
    const TerrainMipMap& getMipMap(int i_lod) const;

  private:
    Sdk::Vector2I d_origin;
    Sdk::Vector2I d_cells;

    std::vector<TerrainMipMap> d_mipMaps;

    void build(const HeightMap& i_heightMap);
  };

} // ns Dx
