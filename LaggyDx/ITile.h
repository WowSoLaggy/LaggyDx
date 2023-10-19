#pragma once

#include "LaggyDxFwd.h"
#include "VolumeUnit.h"


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

      virtual const VolumeUnit& getVolumeUnit() const = 0;
      virtual void setVolumeUnit(VolumeUnit i_volumeUnit) = 0;
    };

  } // ns thd
} // ns Dx
