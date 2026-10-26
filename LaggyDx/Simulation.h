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

      // Flows / diffuses gas between two gas objects (with the heat it carries).
      // Also usable outside of the tiles grid, e.g. for a gas container connected to a tile
      static void exchangeGas(IThdObject& i_obj1, IThdObject& i_obj2, double i_dt);

    private:
      double d_dt = 0;
      const ITileCollection* d_tiles = nullptr;

      void updateAllTiles();
      void updateTile(const Sdk::Vector2I& i_coords);

      void applyNewState();

    private:
      void exchangeHeat(IThdObject& i_obj1, IThdObject& i_obj2);
    };

  } // ns thd
} // ns Dx
