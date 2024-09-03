#pragma once

#include "LaggyDxFwd.h"

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

      void updateAllTiles();
      void updateTile(const Sdk::Vector2I& i_coords);

      void applyNewState();
    };

  } // ns thd
} // ns Dx
