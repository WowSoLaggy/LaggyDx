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
      };

      double d_dt = 0;
      const ITileCollection* d_tiles = nullptr;

      // The tiles of the area by rows, valid during a step. Kept between the steps not to allocate them anew
      std::vector<Cell> d_cells;
      int d_width = 0;

      void collectCells();
      void updateAllTiles();
      void updateCell(int i_index);
      void exchangeWithNeighbor(const Cell& i_cell, const Cell& i_neighbor);

      void applyNewState();

    private:
      void exchangeHeat(IThdObject& i_obj1, IThdObject& i_obj2);
    };

  } // ns thd
} // ns Dx
