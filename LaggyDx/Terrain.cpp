#include "stdafx.h"
#include "Terrain.h"

#include "HeightMap.h"
#include "TerrainChunk.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  Terrain::Terrain(const HeightMap& i_heightMap, const int i_chunkSize)
    : d_chunkSize(i_chunkSize)
  {
    build(i_heightMap);
  }


  const TerrainChunk& Terrain::getChunk(const int i_x, const int i_y) const
  {
    CONTRACT_EXPECT(0 <= i_x && i_x < d_chunksCount.x);
    CONTRACT_EXPECT(0 <= i_y && i_y < d_chunksCount.y);
    return SAFE_DEREF(d_chunks.at(i_x + i_y * d_chunksCount.x));
  }


  void Terrain::build(const HeightMap& i_heightMap)
  {
    CONTRACT_EXPECT(d_chunkSize > 0);

    // A W x H point grid holds (W - 1) x (H - 1) cells to split into chunks.
    const Sdk::Vector2I cells{ i_heightMap.getWidth() - 1, i_heightMap.getHeight() - 1 };
    CONTRACT_EXPECT(cells.x > 0 && cells.y > 0);
    CONTRACT_EXPECT(cells.x % d_chunkSize == 0 && cells.y % d_chunkSize == 0);

    d_chunksCount = {
      (cells.x + d_chunkSize - 1) / d_chunkSize,
      (cells.y + d_chunkSize - 1) / d_chunkSize };

    d_chunks.reserve(d_chunksCount.x * d_chunksCount.y);

    for (int cy = 0; cy < d_chunksCount.y; ++cy)
    {
      for (int cx = 0; cx < d_chunksCount.x; ++cx)
      {
        const Sdk::Vector2I origin{ cx * d_chunkSize, cy * d_chunkSize };

        d_chunks.push_back(std::make_shared<TerrainChunk>(i_heightMap, origin, d_chunkSize));
      }
    }
  }

} // ns Dx
