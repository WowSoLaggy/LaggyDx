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

      double getHeatTransferAmount(const IThdObject& i_obj1, const IThdObject& i_obj2, const double i_dt)
      {
        const auto t1Opt = i_obj1.getTemperature();
        const auto t2Opt = i_obj2.getTemperature();
        if (!t1Opt || !t2Opt)
          return 0;

        const double t1 = *t1Opt;
        const double t2 = *t2Opt;
        const double tDiff = t1 - t2;

        const double thermalConductivity1 = i_obj1.getThermalConductivity();
        const double thermalConductivity2 = i_obj2.getThermalConductivity();
        const double effectiveThermalConductivity = 2 * thermalConductivity1 * thermalConductivity2 /
          (thermalConductivity1 + thermalConductivity2);

        const double heatTransfer = effectiveThermalConductivity * tDiff * i_dt;
        return heatTransfer;
      }

    } // anonym ns

    void Simulation::update(const double i_dt, const ITileCollection& i_tiles)
    {
      d_dt = i_dt;
      d_tiles = &i_tiles;

      exchangeAll();
      storeBufferToTiles(i_dt);
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

      // If tile has objects, exchange heat with them
      heatExchangeWithObjects(i_coords);

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

      heatExchangeWithNeighborTile(*tile1, *tile2, i_coords1);
      gasPressureFlow(*tile1, *tile2, d_buffer[i_coords1]);
    }


    void Simulation::heatExchangeWithNeighborTile(const ITile& i_tile1, const ITile& i_tile2, const Sdk::Vector2I& i_coords1)
    {
      if (!i_tile1.getUnit().hasGas() || !i_tile2.getUnit().hasGas())
        return;

      const double heatTransfer = getHeatTransferAmount(i_tile1, i_tile2, d_dt);
      d_buffer[i_coords1].T -= heatTransfer / i_tile1.getHeatCapacity();
    }

    void Simulation::gasPressureFlow(const ITile& i_tile1, const ITile& i_tile2, BufferTile& io_dst1)
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
        const double giveRatio = std::min(ratioToHalf * d_dt * K, ratioToHalf);
        const auto gasesToShare = i_src1.extractGases(giveRatio);

        io_dst1.unit.removeGases(gasesToShare, true);
      }
      else if (p2 > p1)
      {
        const double ratioToHalf = equilibriumPressure / p2;
        const double giveRatio = std::min(ratioToHalf * d_dt * K, ratioToHalf);
        const auto gasesToShare = i_src2.extractGases(giveRatio);

        io_dst1.unit.addGases(gasesToShare, true);

        // Calculate T change with the gas exchange
        int totalGasShared = 0;
        for (const auto& gas : gasesToShare)
          totalGasShared += gas.second;

        const auto t1Opt = i_tile1.getTemperature();
        const auto t2Opt = i_tile2.getTemperature();
        CONTRACT_ASSERT(t2Opt);
        const double t2 = *t2Opt;

        if (!t1Opt)
          io_dst1.T = t2;
        else
        {
          const double t1 = *t1Opt;
          const double newT = (t1 * i_src1.getGasAmount() + t2 * totalGasShared) / (i_src1.getGasAmount() + totalGasShared);
          io_dst1.T = newT - t1;
        }
      }
    }

    void Simulation::heatExchangeWithObjects(const Sdk::Vector2I& i_coords)
    {
      CONTRACT_EXPECT(d_tiles);

      const auto tilePtr = d_tiles->getTile(i_coords);
      auto& tile = SAFE_DEREF(tilePtr);

      // No gas in tile - no heat exchange with objects
      // (considering that objects don't exchange heat with each other, only via gases)
      if (!tile.getUnit().hasGas())
        return;

      // No tile temperature - no heat exchange with objects
      const auto tOpt = tile.getTemperature();
      if (!tOpt)
        return;

      for (auto* agent : tile.getHeatAgents())
      {
        CONTRACT_ASSERT(agent);
        const auto tAgentOpt = agent->getTemperature();
        if (!tAgentOpt)
          continue;

        const double heatTransfer = getHeatTransferAmount(*agent, tile, d_dt);
        agent->setTemperature(*tAgentOpt - heatTransfer / agent->getHeatCapacity());
        d_buffer[i_coords].T += heatTransfer / tile.getHeatCapacity();
      }
    }


    void Simulation::removeGasIfAirtight(const Sdk::Vector2I& i_coords)
    {
      CONTRACT_EXPECT(d_tiles);

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
    void Simulation::storeBufferToTiles(const double i_dt)
    {
      CONTRACT_EXPECT(d_tiles);

      for (const auto& [coord, tileSrc] : d_buffer)
      {
        auto tileDst = d_tiles->getTile(coord);
        CONTRACT_EXPECT(tileDst);

        const auto curTOpt = tileDst->getTemperature();
        if (!curTOpt)
          tileDst->setTemperature(tileSrc.T);
        else
          tileDst->setTemperature(*tileDst->getTemperature() + tileSrc.T);

        tileDst->getUnit().addGases(tileSrc.unit.getGases());

        tileDst->afterUpdate(i_dt);
      }

      d_buffer.clear();
    }

  } // ns thd
} // ns Dx
