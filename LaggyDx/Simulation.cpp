#include "stdafx.h"
#include "Simulation.h"

#include "GasUnit.h"
#include "ITile.h"
#include "ITileCollection.h"



namespace Dx
{
  namespace thd
  {
    namespace
    {
      constexpr int MaxGasesInExchange = 16;

      struct GasIds
      {
        std::array<GasId, MaxGasesInExchange> ids;
        int count = 0;
      };

      // Both maps are sorted, so are the ids
      GasIds collectGasesForDiffusion(const GasUnit& i_gas1, const GasUnit& i_gas2)
      {
        GasIds res;

        auto it1 = i_gas1.getGases().begin();
        auto it2 = i_gas2.getGases().begin();
        const auto end1 = i_gas1.getGases().end();
        const auto end2 = i_gas2.getGases().end();

        while (it1 != end1 || it2 != end2)
        {
          CONTRACT_ASSERT(res.count < MaxGasesInExchange);

          if (it2 == end2 || (it1 != end1 && it1->first < it2->first))
            res.ids[res.count++] = (it1++)->first;
          else if (it1 == end1 || it2->first < it1->first)
            res.ids[res.count++] = (it2++)->first;
          else
          {
            res.ids[res.count++] = it1->first;
            ++it1;
            ++it2;
          }
        }

        return res;
      }

      int getGasAmount(const GasUnit& i_gas, const GasId i_gasId)
      {
        const auto it = i_gas.getGases().find(i_gasId);
        return it != i_gas.getGases().end() ? it->second : 0;
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

      collectCells();
      updateAllTiles();
      applyNewState();
    }


    void Simulation::collectCells()
    {
      CONTRACT_EXPECT(d_tiles);

      const auto& rect = d_tiles->getRect();
      d_width = rect.width() + 1;
      d_cells.resize((size_t)d_width * (rect.height() + 1));

      int index = 0;
      for (int y = rect.top(); y <= rect.bottom(); ++y)
      {
        for (int x = rect.left(); x <= rect.right(); ++x)
        {
          const auto tile = d_tiles->getTile({ x, y });
          CONTRACT_ASSERT(tile);

          auto& cell = d_cells[index++];
          cell.tile = tile.get();
          cell.all.clear();
          cell.external.clear();
          cell.tile->collectThdObjects(cell.all, cell.external);
        }
      }
    }

    void Simulation::updateAllTiles()
    {
      for (int index = 0; index < (int)d_cells.size(); ++index)
        updateCell(index);
    }

    void Simulation::updateCell(const int i_index)
    {
      const auto& cell = d_cells[i_index];
      cell.tile->beforeExchange();

      // Inside the tile everything exchanges with everything
      for (int j = 0; j < (int)cell.all.size(); ++j)
      {
        for (int i = j + 1; i < (int)cell.all.size(); ++i)
          exchangeHeat(SAFE_DEREF(cell.all[j]), SAFE_DEREF(cell.all[i]));
      }

      // Those at the border of the area have no neighbors to that side
      if ((i_index + 1) % d_width != 0)
        exchangeWithNeighbor(cell, d_cells[i_index + 1]);
      if (i_index + d_width < (int)d_cells.size())
        exchangeWithNeighbor(cell, d_cells[i_index + d_width]);
    }

    void Simulation::exchangeWithNeighbor(const Cell& i_cell, const Cell& i_neighbor)
    {
      // Empty space around has nothing to exchange
      if (i_cell.tile == i_neighbor.tile)
        return;

      i_neighbor.tile->beforeExchange();

      // Between tiles only external objects exchange, so internal ones get heat only via their own tile's external objects
      for (auto* thdObject : i_cell.external)
      {
        for (auto* thdObjectNeighbor : i_neighbor.external)
        {
          exchangeHeat(SAFE_DEREF(thdObject), SAFE_DEREF(thdObjectNeighbor));

          if (thdObject->isGas() && thdObjectNeighbor->isGas())
            exchangeGas(SAFE_DEREF(thdObject), SAFE_DEREF(thdObjectNeighbor), d_dt);
        }
      }
    }

    void Simulation::applyNewState()
    {
      for (const auto& cell : d_cells)
      {
        for (auto* thdObject : cell.all)
          SAFE_DEREF(thdObject).applyTemperature();
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
      if (tDiff == 0)
        return;

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

      const bool hasTrappedGas = gas1.hasTrappedGas() || gas2.hasTrappedGas();

      // Pressure difference -> the gas flows as a whole. Fast: the hole between two tiles is their whole side, and devices
      // (vents, scrubbers) put / take a lot on one tile, which shall spread before it builds up a difference
      // (not for a few Pa: a flow takes at least a unit of each gas, more than that would even out)
      const double PressureThresholdForFlow = 2;
      if (hasTrappedGas || pDiff > PressureThresholdForFlow)
      {
        // Size of the hole to transfer gases apprx in m^2. A pair of 1 m3 tiles gets 2 * 20 / 60 of their difference evened
        // out a step (no more: a tile exchanges with 4 neighbors in a row)
        const double Permeability = 20;

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
        }

        if (gasAmountToFlow > 0)
        {
          const auto gasesToFlow = gasSrc.removeAmountOfGases(gasAmountToFlow);
          addGasesWithHeat(objSrc, objDst, gasesToFlow);
        }
      }

      // Different mixes -> each gas diffuses by its own concentration, whether the gas flows or not (a flow alone carries
      // the mix it has and never evens out the mixes)
      if (hasTrappedGas)
        return;

      const double DiffusionCoefficient = 2;

      const auto gasesForDiffusion = collectGasesForDiffusion(gas1, gas2);
      if (gasesForDiffusion.count < 2)
        return;

      // Both volumes are positive here: a unit with no volume either doesn't participate or has trapped gas
      const double volume1 = gas1.getVolume();
      const double volume2 = gas2.getVolume();

      for (int i = 0; i < gasesForDiffusion.count; ++i)
      {
        const auto gasId = gasesForDiffusion.ids[i];

        // Get gas amount or zero if no gas
        const double amount1 = getGasAmount(gas1, gasId);
        const double amount2 = getGasAmount(gas2, gasId);
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

  } // ns thd
} // ns Dx
