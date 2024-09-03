#include "stdafx.h"
#include "Simulation.h"

#include "ITile.h"
#include "ITileCollection.h"

#include <LaggySdk/Shuffle.h>


namespace Dx
{
  namespace thd
  {
    namespace
    {
      const std::vector<Sdk::Vector2I> getCoordsToExchange(const Sdk::Vector2I& i_coords)
      {
        return { i_coords + Sdk::Vector2I{ 1, 0 },
                 i_coords + Sdk::Vector2I{ 0, 1 } };
      }

      void exchangeHeat(IThdObject& i_obj1, IThdObject& i_obj2, const double i_dt)
      {
        const double t1 = i_obj1.getTemperature();
        const double t2 = i_obj2.getTemperature();
        const double tDiff = t2 - t1;

        const double thermalConductivity1 = i_obj1.getThermalConductivity();
        const double thermalConductivity2 = i_obj2.getThermalConductivity();
        const double thermalConductivityEffective = thermalConductivity1 + thermalConductivity2 / 2;

        const double heatTransfer = thermalConductivityEffective * tDiff * i_dt;
        i_obj1.transferHeat(heatTransfer);
        i_obj2.transferHeat(-heatTransfer);
      }

    } // anonym ns

    void Simulation::update(const double i_dt, const ITileCollection& i_tiles)
    {
      d_dt = i_dt;
      d_tiles = &i_tiles;

      updateAllTiles();
      applyNewState();
    }


    void Simulation::updateAllTiles()
    {
      CONTRACT_EXPECT(d_tiles);

      const auto& rect = d_tiles->getRect();
      for (int y = rect.top(); y <= rect.bottom(); ++y)
      {
        for (int x = rect.left(); x <= rect.right(); ++x)
          updateTile({ x, y });
      }
    }

    void Simulation::updateTile(const Sdk::Vector2I& i_coords)
    {
      const auto tile = d_tiles->getTile(i_coords);
      CONTRACT_ASSERT(tile);

      const auto& thdObjects = tile->getThdObjects();
      for (int j = 0; j < thdObjects.size(); ++j)
      {
        for (int i = j + 1; i < thdObjects.size(); ++i)
          exchangeHeat(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjects[i]), d_dt);

        for (const auto& coords : getCoordsToExchange(i_coords))
        {
          const auto tileNeighbor = d_tiles->getTile(i_coords);
          if (!tileNeighbor)
            continue; // Skip if the neighbor is out of the area

          const auto& thdObjectsNeighbor = tileNeighbor->getThdObjects();
          for (const auto& thdObjectNeighbor : thdObjectsNeighbor)
            exchangeHeat(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjectNeighbor), d_dt);
        }
      }
    }

    //void Simulation::gasPressureFlow(const ITile& i_tile1, const ITile& i_tile2, BufferTile& io_dst1)
    //{
    //  if (i_tile1.isAirTight() || i_tile2.isAirTight())
    //    return;

    //  const auto i_src1 = i_tile1.getUnit();
    //  const auto i_src2 = i_tile2.getUnit();
    //  if (!i_src1.hasGas() && !i_src2.hasGas())
    //    return;

    //  const double p1 = i_src1.getPressure();
    //  const double p2 = i_src2.getPressure();
    //  const double pressureDifference = std::abs(p2 - p1);
    //  if (pressureDifference <= 2)
    //    return;
    //  const double equilibriumPressure = pressureDifference / 2.0;

    //  constexpr double K = 4.0;

    //  if (p1 > p2)
    //  {
    //    const double ratioToHalf = equilibriumPressure / p1;
    //    const double giveRatio = std::min(ratioToHalf * d_dt * K, ratioToHalf);
    //    const auto gasesToShare = i_src1.extractGases(giveRatio);

    //    io_dst1.unit.removeGases(gasesToShare, true);
    //  }
    //  else if (p2 > p1)
    //  {
    //    const double ratioToHalf = equilibriumPressure / p2;
    //    const double giveRatio = std::min(ratioToHalf * d_dt * K, ratioToHalf);
    //    const auto gasesToShare = i_src2.extractGases(giveRatio);

    //    io_dst1.unit.addGases(gasesToShare, true);

    //    // Calculate T change with the gas exchange
    //    int totalGasShared = 0;
    //    for (const auto& gas : gasesToShare)
    //      totalGasShared += gas.second;

    //    const auto t1Opt = i_tile1.getTemperature();
    //    const auto t2Opt = i_tile2.getTemperature();
    //    CONTRACT_ASSERT(t2Opt);
    //    const double t2 = *t2Opt;

    //    if (!t1Opt)
    //      io_dst1.T = t2;
    //    else
    //    {
    //      const double t1 = *t1Opt;
    //      const double newT = (t1 * i_src1.getGasAmount() + t2 * totalGasShared) / (i_src1.getGasAmount() + totalGasShared);
    //      io_dst1.T = newT - t1;
    //    }
    //  }
    //}


    /// Store the buffer values into the tiles
    void Simulation::applyNewState()
    {
      CONTRACT_EXPECT(d_tiles);

      const auto& rect = d_tiles->getRect();
      for (int y = rect.top(); y <= rect.bottom(); ++y)
      {
        for (int x = rect.left(); x <= rect.right(); ++x)
        {
          auto tile = d_tiles->getTile({ x, y });
          CONTRACT_ASSERT(tile);

          for (const auto& thdObject : tile->getThdObjects())
          {
            CONTRACT_ASSERT(thdObject);
            thdObject->applyTemperature();
          }
        }
      }
    }

  } // ns thd
} // ns Dx
