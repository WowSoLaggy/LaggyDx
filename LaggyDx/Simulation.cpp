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


    void Simulation::exchange(Unit& io_unit1, Unit& io_unit2)
    {
      exchange(io_unit1, io_unit2, io_unit1, io_unit2);
    }


    void Simulation::exchangeAll()
    {
      CONTRACT_EXPECT(d_tiles);
      CONTRACT_EXPECT(d_buffer.empty());

      const auto& rect = d_tiles->getRect();
      for (int y = rect.top(); y < rect.bottom(); ++y)
      {
        for (int x = rect.left(); x < rect.right(); ++x)
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

      exchange(tile1->getUnit(), tile2->getUnit(), d_buffer[i_coords1].unit, d_buffer[i_coords2].unit);

      const double t1 = tile1->getT();
      const double t2 = tile2->getT();
      const double tDiff = t2 - t1;

      constexpr double K = 0.05;
      const double tChange = tDiff * d_dt * K;

      d_buffer[i_coords1].T += tChange;
      d_buffer[i_coords2].T -= tChange;
    }

    void Simulation::exchange(const Unit& i_src1, const Unit& i_src2, Unit& io_dst1, Unit& io_dst2)
    {
      if (!i_src1.hasGas() && !i_src2.hasGas())
        return;

      const double p1 = i_src1.getPressure();
      const double p2 = i_src2.getPressure();
      const double pDiffHalf = std::abs(p2 - p1) / 2.0;

      constexpr double K = 0.5;

      if (p1 > p2)
      {
        const double giveRatio = pDiffHalf / p1 * d_dt * K;
        const auto gasesToShare = i_src1.extractGases(giveRatio);

        io_dst1.removeGases(gasesToShare, true);
        io_dst2.addGases(gasesToShare, true);
      }
      else
      {
        const double giveRatio = pDiffHalf / p2 * d_dt * K;
        const auto gasesToShare = i_src2.extractGases(giveRatio);

        io_dst2.removeGases(gasesToShare, true);
        io_dst1.addGases(gasesToShare, true);
      }
    }


    void Simulation::storeBuffer()
    {
      CONTRACT_EXPECT(d_tiles);

      for (const auto& [coord, tileSrc] : d_buffer)
      {
        auto tileDst = d_tiles->getTile(coord);
        CONTRACT_EXPECT(tileDst);

        tileDst->setT(tileDst->getT() + tileSrc.T);
        tileDst->getUnit().addGases(tileSrc.unit.getGases());
      }

      d_buffer.clear();
    }

  } // ns thd
} // ns Dx
