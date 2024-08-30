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
        return { i_coords + Sdk::Vector2I{ -1, 0 },
                 i_coords + Sdk::Vector2I{ 0, -1 },
                 i_coords + Sdk::Vector2I{ 1, 0 },
                 i_coords + Sdk::Vector2I{ 0, 1 } };
      }

      const std::vector<Sdk::Vector2I> getShuffledCoordsToExchange(const Sdk::Vector2I& i_coords)
      {
        auto coords = getCoordsToExchange(i_coords);
        Sdk::shuffleVector(coords);
        return coords;
      }

    } // anonym ns

    void Simulation::update(const double i_dt, const ITileCollection& i_tiles)
    {
      d_dt = i_dt;
      d_tiles = &i_tiles;

      exchangeAll();
      storeBufferToTiles();
    }


    void Simulation::exchangeAll()
    {
      CONTRACT_EXPECT(d_tiles);
      CONTRACT_EXPECT(d_buffer.empty());

      const auto& rect = d_tiles->getRect();
      for (int y = rect.top(); y <= rect.bottom(); ++y)
      {
        for (int x = rect.left(); x <= rect.right(); ++x)
        {
          if (x == 8 && y == 8)
          {
            int a = 0;
            a = 10;
          }
          exchangeForTileAtCoords({ x, y });
        }
      }
    }

    void Simulation::exchangeForTileAtCoords(const Sdk::Vector2I& i_coords)
    {
      for (const auto& coords : getShuffledCoordsToExchange(i_coords))
        exchangeBetweenCoords(i_coords, coords);

      // If tile is airtight but still has gases, exchange them with neighbors
      removeGasIfAirtight(i_coords);
    }

    
    void Simulation::exchangeBetweenCoords(const Sdk::Vector2I& i_coords1, const Sdk::Vector2I& i_coords2)
    {
      CONTRACT_EXPECT(d_tiles);

      const auto tile1 = d_tiles->getTile(i_coords1);
      const auto tile2 = d_tiles->getTile(i_coords2);
      CONTRACT_EXPECT(tile1);
      CONTRACT_EXPECT(tile2);

      heatExchange(*tile1, *tile2, i_coords1);
      gasExchange(*tile1, *tile2, d_buffer[i_coords1]);
    }


    void Simulation::heatExchange(const ITile& i_tile1, const ITile& i_tile2, const Sdk::Vector2I& i_coords1)
    {
      const double t1 = i_tile1.getT();
      const double t2 = i_tile2.getT();
      const double tDiff = t2 - t1;

      const double insulationFactor1 = i_tile1.getInsulationFactor();
      const double insulationFactor2 = i_tile2.getInsulationFactor();
      const double effectiveInsulationFactor = std::min(insulationFactor1, insulationFactor2);

      constexpr double K = 0.05;
      const double tChange = K * tDiff * d_dt * effectiveInsulationFactor;

      d_buffer[i_coords1].T += tChange;
    }

    void Simulation::gasExchange(const ITile& i_tile1, const ITile& i_tile2, BufferTile& io_dst1)
    {
      if (i_tile1.isAirTight() || i_tile2.isAirTight())
        return;

      const auto i_src1 = i_tile1.getUnit();
      const auto i_src2 = i_tile2.getUnit();
      if (!i_src1.hasGas() && !i_src2.hasGas())
        return;

      const double p1 = i_src1.getPressure();
      const double p2 = i_src2.getPressure();
      const double pressureDifference = std::abs(p2 - p1);
      if (pressureDifference <= 2)
        return;
      const double equilibriumPressure = pressureDifference / 2.0;

      constexpr double K = 4.0;

      if (p1 > p2)
      {
        const double ratioToHalf = equilibriumPressure / p1;
        const double maxRatio = ratioToHalf / 4; // to be sure that all 4 neightbors will get some gas
        const double giveRatio = std::min(ratioToHalf * d_dt * K, maxRatio);
        const auto gasesToShare = i_src1.extractGases(giveRatio);

        io_dst1.unit.removeGases(gasesToShare, true);
      }
      else
      {
        const double ratioToHalf = equilibriumPressure / p2;
        const double maxRatio = ratioToHalf / 4; // to be sure that all 4 neightbors will get some gas
        const double giveRatio = std::min(ratioToHalf * d_dt * K, maxRatio);
        const auto gasesToShare = i_src2.extractGases(giveRatio);

        io_dst1.unit.addGases(gasesToShare, true);

        // Calculate T change with the gas exchange
        int totalGasShared = 0;
        for (const auto& gas : gasesToShare)
          totalGasShared += gas.second;

        const double newT = (i_tile1.getT() * i_src1.getGasAmount() + i_tile2.getT() * totalGasShared) / (i_src1.getGasAmount() + totalGasShared);
        io_dst1.T = newT - i_tile1.getT();
      }
    }


    void Simulation::removeGasIfAirtight(const Sdk::Vector2I& i_coords)
    {
      const auto tilePtr = d_tiles->getTile(i_coords);
      auto& tile = SAFE_DEREF(tilePtr);

      if (!tile.isAirTight() || !tile.getUnit().hasGas())
        return;

      // Collect coords of neighbors that are not airtight
      std::vector<Sdk::Vector2I> coordsToExchange;
      for (const auto& coords : getCoordsToExchange(i_coords))
      {
        const auto neighTilePtr = d_tiles->getTile(coords);
        const auto& neighTile = SAFE_DEREF(neighTilePtr);
        if (!neighTile.isAirTight())
          coordsToExchange.push_back(coords);
      }

      if (!coordsToExchange.empty())
      {
        const double giveRatio = 1.0 / coordsToExchange.size();
        for (const auto& coords : coordsToExchange)
        {
          const auto gasesToShare = tile.getUnit().extractGases(giveRatio);

          auto& bufferUnit = d_buffer[coords].unit;
          bufferUnit.addGases(gasesToShare, true);
        }
      }

      tile.getUnit().clear();
    }


    /// Store the buffer values into the tiles
    void Simulation::storeBufferToTiles()
    {
      CONTRACT_EXPECT(d_tiles);

      for (const auto& [coord, tileSrc] : d_buffer)
      {
        auto tileDst = d_tiles->getTile(coord);
        CONTRACT_EXPECT(tileDst);

        tileDst->setT(tileDst->getT() + tileSrc.T);
        tileDst->getUnit().addGases(tileSrc.unit.getGases());

        tileDst->afterUpdate();
      }

      d_buffer.clear();
    }

  } // ns thd
} // ns Dx
