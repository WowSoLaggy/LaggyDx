#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // A height map split into a grid of TerrainChunks, each carrying its own LODs.
  class Terrain
  {
  public:
    static constexpr int c_defaultChunkSize = 64;

    // \param i_chunkSize - chunk edge length in grid cells
    Terrain(
      const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice,
      int i_chunkSize = c_defaultChunkSize);

    const Sdk::Vector2I& getChunksCount() const { return d_chunksCount; }

    int getChunksSize() const { return (int)d_chunks.size(); }
    const TerrainChunk& getChunk(int i_x, int i_y) const;

  private:
    Sdk::Vector2I d_chunksCount;
    std::vector<std::unique_ptr<TerrainChunk>> d_chunks;

    void build(const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice, int i_chunkSize);
  };

} // ns Dx
