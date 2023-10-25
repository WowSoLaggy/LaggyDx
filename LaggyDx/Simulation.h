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
      void exchange(Unit& io_unit1, Unit& io_unit2);

    private:
      double d_dt = 0;
      const ITileCollection* d_tiles = nullptr;

      using BufferTilesMap = std::unordered_map<Sdk::Vector2I, BufferTile, Sdk::Vector2_hash>;
      BufferTilesMap d_buffer;

      void exchangeAll();
      void exchangeAtCoords(const Sdk::Vector2I& i_coords);
      void exchange(const Sdk::Vector2I& i_coords1, const Sdk::Vector2I& i_coords2);
      void exchange(const Unit& i_src1, const Unit& i_src2, Unit& io_dst1, Unit& io_dst2);

      void storeBuffer();
    };

  } // ns thd
} // ns Dx
