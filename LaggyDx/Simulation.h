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
      struct Cell
      {
        const ITile* tile = nullptr;
        std::vector<IThdObject*> all;
        std::vector<IThdObject*> external;
        // The gas ones of the external, the only ones gas flows between
        std::vector<IThdObject*> externalGas;
      };

      // Heat spreads slowly (the conductivity over the heat capacity is ~1e-3 per second at most), so a tile exchanges it
      // once in this many steps with that many steps' time - the work is the same for every step, a part of the tiles
      // each. Gas needs every step: it flows fast
      static constexpr int HeatEveryNSteps = 10;

      double d_dt = 0;
      double d_heatDt = 0;
      int d_step = 0;
      const ITileCollection* d_tiles = nullptr;

      // The tiles of the area by rows, valid during a step. Kept between the steps not to allocate them anew
      std::vector<Cell> d_cells;
      int d_width = 0;

      void collectCells();
      void updateAllTiles();
      void updateCell(int i_index, bool i_heat);
      void exchangeWithNeighbor(const Cell& i_cell, const Cell& i_neighbor, bool i_heat);

      void applyNewState();

    private:
      void exchangeHeat(IThdObject& i_obj1, IThdObject& i_obj2);
    };

  } // ns thd
} // ns Dx
