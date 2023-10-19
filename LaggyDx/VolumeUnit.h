#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  namespace thd
  {
    class VolumeUnit
    {
    public:
      void setVolume(double i_volume);
      double getVolume() const;

      double getPressure() const;

    private:
      double d_volume = 1;
      GasesMap d_gases;
    };

  } // ns thd
} // ns Dx
