#include "stdafx.h"
#include "Simulation.h"

#include "ITile.h"
#include "ITileCollection.h"


namespace Dx
{
  namespace thd
  {
    void Simulation::update(const double i_dt, const ITileCollection& i_tiles)
    {
      d_dt = i_dt;
      d_tiles = &i_tiles;

      exchangeAll();
      storeBuffer();
    }


    void Simulation::exchangeAll()
    {
      CONTRACT_EXPECT(d_tiles);
      CONTRACT_EXPECT(d_buffer.empty());

      const int minX = d_tiles->minX();
      const int minY = d_tiles->minY();
      const int maxX = d_tiles->maxX();
      const int maxY = d_tiles->maxY();

      for (int y = minY; y < maxY; ++y)
      {
        for (int x = minX; x < maxX; ++x)
          exchangeAtCoords({ x, y });
      }
    }

    void Simulation::exchangeAtCoords(const Sdk::Vector2I& i_coords)
    {
      exchange(i_coords, i_coords + Sdk::Vector2I{ 1, 0 });
      exchange(i_coords, i_coords + Sdk::Vector2I{ 0, 1 });
    }

    void Simulation::exchange(const Sdk::Vector2I& i_coords1, const Sdk::Vector2I& i_coords2)
    {
      CONTRACT_EXPECT(d_tiles);

      const auto tile1 = d_tiles->getTile(i_coords1);
      const auto tile2 = d_tiles->getTile(i_coords2);
      CONTRACT_EXPECT(tile1);
      CONTRACT_EXPECT(tile2);

      const double t1 = tile1->getT();
      const double t2 = tile2->getT();
      const double tDiff = t2 - t1;

      constexpr double K = 0.05;
      const double tChange = tDiff * d_dt * K;

      d_buffer[i_coords1].T += tChange;
      d_buffer[i_coords2].T -= tChange;
    }


    void Simulation::storeBuffer()
    {
      CONTRACT_EXPECT(d_tiles);

      for (const auto& [coord, tileSrc] : d_buffer)
      {
        auto tileDst = d_tiles->getTile(coord);
        CONTRACT_EXPECT(tileDst);
        tileDst->setT(tileDst->getT() + tileSrc.T);
      }

      d_buffer.clear();
    }

  } // ns thd
} // ns Dx
