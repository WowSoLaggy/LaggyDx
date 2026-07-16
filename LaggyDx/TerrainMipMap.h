#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // One LOD of one TerrainChunk - a mesh sampled from the height map at a fixed grid stride.
  class TerrainMipMap
  {
  public:
    // \param i_origin - chunk's top-left corner in height map grid coords
    // \param i_cells - chunk's size in grid cells (mesh spans i_cells + 1 points per axis at full res)
    // \param i_step - LOD stride in grid points (1 = full res, 2 = half, 4 = quarter, ...)
    TerrainMipMap(
      const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells, int i_step);

    int getStep() const { return d_step; }
    const std::shared_ptr<IObject3>& getObject() const { return d_object; }

  private:
    int d_step = 1;
    std::shared_ptr<IObject3> d_object;

    void build(
      const HeightMap& i_heightMap, const Sdk::Vector2I& i_origin, const Sdk::Vector2I& i_cells);
  };

} // ns Dx
