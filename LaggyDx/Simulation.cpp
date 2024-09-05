#include "stdafx.h"
#include "Simulation.h"

#include "GasUnit.h"
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
          exchangeHeat(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjects[i]));

        for (const auto& coords : getCoordsToExchange(i_coords))
        {
          const auto tileNeighbor = d_tiles->getTile(coords);
          if (!tileNeighbor)
            continue; // Skip if the neighbor is out of the area

          const auto& thdObjectsNeighbor = tileNeighbor->getThdObjects();
          for (const auto& thdObjectNeighbor : thdObjectsNeighbor)
          {
            exchangeHeat(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjectNeighbor));

            if (thdObjects[j]->isGas() && thdObjectNeighbor->isGas())
              exchangeGas(SAFE_DEREF(thdObjects[j]), SAFE_DEREF(thdObjectNeighbor));
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

          for (const auto& thdObject : tile->getThdObjects())
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

    void Simulation::exchangeGas(IThdObject& i_obj1, IThdObject& i_obj2)
    {
      auto& gas1 = SAFE_DEREF(i_obj1.getGasUnit());
      auto& gas2 = SAFE_DEREF(i_obj2.getGasUnit());

      const double p1 = gas1.getPressure();
      const double p2 = gas2.getPressure();
      const double pDiff = std::abs(p2 - p1);

      const double PressureThresholdForFlow = 2;
      const double Permeability = 2; // Size of the hole to transfer gases apprx in m^2

      if (pDiff > PressureThresholdForFlow)
      {
        // Significant pressure difference -> gases flow dominant
        const int gasAmountToFlow = (int)std::ceil(Permeability * pDiff * d_dt);

        auto& gasSrc = p1 > p2 ? gas1 : gas2;
        auto& gasDst = p1 > p2 ? gas2 : gas1;
        auto& objSrc = p1 > p2 ? i_obj1 : i_obj2;
        auto& objDst = p1 > p2 ? i_obj2 : i_obj1;

        const double g2 = gasDst.getGasAmount();
        const double t2 = objDst.getTemperature();

        const auto gasesToFlow = gasSrc.removeAmountOfGases(gasAmountToFlow);
        gasDst.addGases(gasesToFlow);

        const double dG = gasDst.getGasAmount() - g2;
        const double t1 = objSrc.getTemperature();

        const double tNew = (g2 * t2 + dG * t1) / (g2 + dG);
        objDst.setTemperature(tNew);
      }
      else
      {
        // Minimal pressure difference -> diffusion dominates
      }
    }

  } // ns thd
} // ns Dx
