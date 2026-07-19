#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // A height map split into a grid of TerrainChunks, each carrying its own LODs.
  class Terrain
  {
  public:
    static constexpr int DefaultChunkSize = 64;

    // \param i_chunkSize - chunk edge length in grid cells
    // \param i_lodsCount - number of LODs to build per chunk (1 = finest only)
    Terrain(const HeightMap& i_heightMap, int i_chunkSize = DefaultChunkSize, int i_lodsCount = 1);

    const Sdk::Vector2I& getChunksCount() const { return d_chunksCount; }

    int getChunksSize() const { return (int)d_chunks.size(); }
    const TerrainChunk& getChunk(int i_x, int i_y) const;

  private:
    const int d_chunkSize;

    Sdk::Vector2I d_chunksCount;
    std::vector<std::shared_ptr<TerrainChunk>> d_chunks;

    void build(const HeightMap& i_heightMap, int i_lodsCount);
  };

} // ns Dx
