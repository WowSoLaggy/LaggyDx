#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  namespace thd
  {
    class ITile
    {
    public:
      virtual ~ITile() = default;

      virtual double getT() const = 0;
      virtual void setT(double i_t) = 0;

      virtual double getInsulationFactor() const = 0; // 0 - perfect insulation, 1 - perfect heat conduction

      virtual Unit& getUnit() = 0;
      virtual const Unit& getUnit() const = 0;
    };

  } // ns thd
} // ns Dx
