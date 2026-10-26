#include "stdafx.h"
#include "Simulation.h"

#include "GasUnit.h"
#include "ITile.h"
#include "ITileCollection.h"

#include <LaggySdk/MapUtils.h>
#include <LaggySdk/Shuffle.h>
#include <LaggySdk/VectorUtils.h>


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

      const std::set<GasId> collectGasesForDiffusion(const GasUnit& i_gas1, const GasUnit& i_gas2)
      {
        const auto gases1 = Sdk::keys(i_gas1.getGases());
        const auto gases2 = Sdk::keys(i_gas2.getGases());

        return Sdk::mergeVectorsToSet(gases1, gases2);
      }

      // Adds the gases (already removed from the source) to the destination together with their heat
      void addGasesWithHeat(const IThdObject& i_src, IThdObject& io_dst, const GasesMap& i_gases)
      {
        auto& gasDst = SAFE_DEREF(io_dst.getGasUnit());

        const bool dstHadGas = gasDst.hasGas();
        const double heatCapacityBefore = io_dst.getHeatCapacity();

        gasDst.addGases(i_gases);

        if (!dstHadGas)
        {
          // Nothing to mix with. Empty gas doesn't exchange heat, so there is nothing accumulated to keep either
          io_dst.setTemperatureIntermediate(i_src.getTemperature());
          return;
        }

        // The arrived gas brings its heat. Transferring it (instead of setting the mixed temperature)
        // keeps the heat accumulated by other exchanges during this step.
        // transferHeat() divides by the current heat capacity that already includes the arrived gas,
        // so the result is the heat-capacity-weighted mix of both temperatures
        const double heatCapacityArrived = io_dst.getHeatCapacity() - heatCapacityBefore;
        io_dst.transferHeat(heatCapacityArrived * (i_src.getTemperature() - io_dst.getTemperature()));
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

      // Inside the tile everything exchanges with everything
      const auto& thdObjects = tile->getThdObjectsAll();
      for (int j = 0; j < thdObjects.size(); ++j)
      {
        for (int i = j + 1; i < thdObjects.size(); ++i)
          exchangeHeat(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjects[i]));
      }

      // Between tiles only external objects exchange, so internal ones get heat only via their own tile's external objects
      const auto& thdObjectsExternal = tile->getThdObjectsExternal();
      for (const auto& coords : getCoordsToExchange(i_coords))
      {
        const auto tileNeighbor = d_tiles->getTile(coords);
        if (!tileNeighbor)
          continue; // Skip if the neighbor is out of the area

        const auto& thdObjectsNeighbor = tileNeighbor->getThdObjectsExternal();
        for (const auto& thdObject : thdObjectsExternal)
        {
          for (const auto& thdObjectNeighbor : thdObjectsNeighbor)
          {
            exchangeHeat(SAFE_DEREF(thdObject), SAFE_DEREF(thdObjectNeighbor));

            if (thdObject->isGas() && thdObjectNeighbor->isGas())
              exchangeGas(SAFE_DEREF(thdObject), SAFE_DEREF(thdObjectNeighbor), d_dt);
          }
        }
      }
    }

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

          for (const auto& thdObject : tile->getThdObjectsAll())
          {
            CONTRACT_ASSERT(thdObject);
            thdObject->applyTemperature();
          }
        }
      }
    }


    void Simulation::exchangeHeat(IThdObject& i_obj1, IThdObject& i_obj2)
    {
      if (const auto gasUnit = i_obj1.getGasUnit(); gasUnit && gasUnit->getPressure() < 100)
        return;
      if (const auto gasUnit = i_obj2.getGasUnit(); gasUnit && gasUnit->getPressure() < 100)
        return;

      const double t1 = i_obj1.getTemperature();
      const double t2 = i_obj2.getTemperature();
      const double tDiff = t2 - t1;

      const double thermalConductivity = std::min(i_obj1.getThermalConductivity(), i_obj2.getThermalConductivity());

      const double heatTransfer = thermalConductivity * tDiff * d_dt;
      i_obj1.transferHeat(heatTransfer);
      i_obj2.transferHeat(-heatTransfer);
    }

    void Simulation::exchangeGas(IThdObject& i_obj1, IThdObject& i_obj2, const double i_dt)
    {
      auto& gas1 = SAFE_DEREF(i_obj1.getGasUnit());
      auto& gas2 = SAFE_DEREF(i_obj2.getGasUnit());

      if (!gas1.participateInGasSimulation() || !gas2.participateInGasSimulation())
        return;

      const double p1 = gas1.getPressure();
      const double p2 = gas2.getPressure();
      const double pDiff = std::abs(p2 - p1);

      const double PressureThresholdForFlow = 2;

      const bool hasTrappedGas = gas1.hasTrappedGas() || gas2.hasTrappedGas();

      if (hasTrappedGas || pDiff > PressureThresholdForFlow)
      {
        // Significant pressure difference -> gases flow dominant

        const double Permeability = 2; // Size of the hole to transfer gases apprx in m^2

        auto& gasSrc = p1 > p2 ? gas1 : gas2;
        auto& gasDst = p1 > p2 ? gas2 : gas1;
        auto& objSrc = p1 > p2 ? i_obj1 : i_obj2;
        auto& objDst = p1 > p2 ? i_obj2 : i_obj1;

        int gasAmountToFlow = std::numeric_limits<int>::max();
        if (!hasTrappedGas)
        {
          // Never flow past equal pressures, otherwise small volumes (e.g. a closing door) make it oscillate
          const double vSrc = gasSrc.getVolume();
          const double vDst = gasDst.getVolume();
          const double amountToEqualize = (gasSrc.getGasAmount() * vDst - gasDst.getGasAmount() * vSrc) / (vSrc + vDst);
          gasAmountToFlow = (int)std::min(std::ceil(Permeability * pDiff * i_dt), std::floor(amountToEqualize));
          if (gasAmountToFlow <= 0)
            return;
        }

        const auto gasesToFlow = gasSrc.removeAmountOfGases(gasAmountToFlow);
        addGasesWithHeat(objSrc, objDst, gasesToFlow);
      }
      else
      {
        // Minimal pressure difference -> diffusion dominates

        const double DiffusionCoefficient = 0.1; // Diffusion coefficient

        const auto gasesForDiffusion = collectGasesForDiffusion(gas1, gas2);
        if (gasesForDiffusion.size() < 2)
          return;

        // Both volumes are positive here: a unit with no volume either doesn't participate or has trapped gas (flow)
        const double volume1 = gas1.getVolume();
        const double volume2 = gas2.getVolume();

        for (const auto gasId : gasesForDiffusion)
        {
          // Get gas amount or zero if no gas
          const double amount1 = gas1.getGases().count(gasId) ? gas1.getGases().at(gasId) : 0;
          const double amount2 = gas2.getGases().count(gasId) ? gas2.getGases().at(gasId) : 0;
          const double concentrationDiff = amount1 / volume1 - amount2 / volume2;

          // Amount that makes both concentrations equal
          const double amountToEqualize = std::abs(amount1 * volume2 - amount2 * volume1) / (volume1 + volume2);
          const double amountExact = std::abs(DiffusionCoefficient * concentrationDiff * i_dt);

          // Round the magnitude up so small differences still diffuse (the same way in both directions),
          // but never move past equal concentrations to avoid oscillation
          const int amountToMove = std::min((int)std::ceil(amountExact), (int)amountToEqualize);
          if (amountToMove <= 0)
            continue;

          auto& gasSrc = concentrationDiff > 0 ? gas1 : gas2;
          auto& objSrc = concentrationDiff > 0 ? i_obj1 : i_obj2;
          auto& objDst = concentrationDiff > 0 ? i_obj2 : i_obj1;

          const int amountRemoved = gasSrc.removeGas(gasId, amountToMove);
          addGasesWithHeat(objSrc, objDst, { { gasId, amountRemoved } });
        }
      }
    }

  } // ns thd
} // ns Dx
