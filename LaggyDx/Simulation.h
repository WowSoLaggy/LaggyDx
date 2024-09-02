#pragma once

#include "LaggyDxFwd.h"
#include "BufferTile.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  namespace thd
  {
    class Simulation
    {
    public:
      void update(double i_dt, const ITileCollection& i_tiles);

    private:
      double d_dt = 0;
      const ITileCollection* d_tiles = nullptr;

      using BufferTilesMap = std::unordered_map<Sdk::Vector2I, BufferTile, Sdk::Vector2_hash>;
      BufferTilesMap d_buffer;

      void exchangeAll();
      void exchangeForTileAtCoords(const Sdk::Vector2I& i_coords);
      void exchangeBetweenCoords(const Sdk::Vector2I& i_coords1, const Sdk::Vector2I& i_coords2);
      void heatExchangeWithObjects(const Sdk::Vector2I& i_coords);

      void heatExchangeWithNeighborTile(const ITile& i_tile1, const ITile& i_tile2, const Sdk::Vector2I& i_coords1);
      void gasPressureFlow(const ITile& i_tile1, const ITile& i_tile2, BufferTile& io_dst1);

      void removeGasIfAirtight(const Sdk::Vector2I& i_coords);

      void storeBufferToTiles(double i_dt);
    };

  } // ns thd
} // ns Dx
