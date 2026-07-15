#include "stdafx.h"
#include "Terrain.h"

#include "HeightMap.h"
#include "TerrainChunk.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  Terrain::Terrain(
    const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice, const int i_chunkSize)
  {
    CONTRACT_EXPECT(i_chunkSize > 0);
    build(i_heightMap, i_renderDevice, i_chunkSize);
  }


  const TerrainChunk& Terrain::getChunk(const int i_x, const int i_y) const
  {
    CONTRACT_EXPECT(0 <= i_x && i_x < d_chunksCount.x);
    CONTRACT_EXPECT(0 <= i_y && i_y < d_chunksCount.y);
    return *d_chunks[i_x + i_y * d_chunksCount.x];
  }


  void Terrain::build(
    const HeightMap& i_heightMap, const IRenderDevice& i_renderDevice, const int i_chunkSize)
  {
    // A W x H point grid holds (W - 1) x (H - 1) cells to split into chunks.
    const Sdk::Vector2I cells{ i_heightMap.getWidth() - 1, i_heightMap.getHeight() - 1 };
    CONTRACT_EXPECT(cells.x > 0 && cells.y > 0);

    d_chunksCount = {
      (cells.x + i_chunkSize - 1) / i_chunkSize,
      (cells.y + i_chunkSize - 1) / i_chunkSize };

    d_chunks.reserve((size_t)d_chunksCount.x * d_chunksCount.y);

    for (int cy = 0; cy < d_chunksCount.y; ++cy)
    {
      for (int cx = 0; cx < d_chunksCount.x; ++cx)
      {
        const Sdk::Vector2I origin{ cx * i_chunkSize, cy * i_chunkSize };
        const Sdk::Vector2I chunkCells{
          std::min(i_chunkSize, cells.x - origin.x),
          std::min(i_chunkSize, cells.y - origin.y) };

        d_chunks.push_back(std::make_unique<TerrainChunk>(
          i_heightMap, i_renderDevice, origin, chunkCells));
      }
    }
  }

} // ns Dx
