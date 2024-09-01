#pragma once

#include "IThdObject.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  namespace thd
  {
    class ITile : public IThdObject
    {
    public:
      virtual bool isAirTight() const = 0;

      virtual Unit& getUnit() = 0;
      virtual const Unit& getUnit() const = 0;

      virtual std::vector<IThdObject*> getHeatAgents() const = 0;

      virtual void afterUpdate(double i_dt) {};
    };

  } // ns thd
} // ns Dx
