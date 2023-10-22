#include "stdafx.h"
#include "VolumeUnit.h"


namespace Dx
{
  namespace thd
  {
    void VolumeUnit::pushFields()
    {
      pushField("volume", d_volume);
    }

    void VolumeUnit::setVolume(const double i_volume)
    {
      d_volume = i_volume;
    }

    double VolumeUnit::getVolume() const
    {
      return d_volume;
    }


    double VolumeUnit::getPressure() const
    {
      int gasesSum = 0;
      for (const auto& [_, count] : d_gases)
        gasesSum += count;

      return gasesSum / d_volume;
    }


    void VolumeUnit::addGas(const GasId i_gasId, const int i_count)
    {
      d_gases[i_gasId] += i_count;
    }

  } // ns thd
} // ns Dx
