#include "stdafx.h"
#include "IThdObject.h"


namespace Dx
{
  namespace thd
  {
    bool IThdObject::isGas() const
    {
      return getGasUnit() != nullptr;
    }

    GasUnit* IThdObject::getGasUnit()
    {
      return nullptr;
    }
    const GasUnit* IThdObject::getGasUnit() const
    {
      return nullptr;
    }


    void IThdObject::transferHeat(const double i_heatAmount)
    {
      if (!d_temperatureIntermediate)
        d_temperatureIntermediate = getTemperature();

      *d_temperatureIntermediate += i_heatAmount / getHeatCapacity();
    }

    void IThdObject::applyTemperature()
    {
      if (d_temperatureIntermediate)
        setTemperature(*d_temperatureIntermediate);
    }

  } // ns thd
} // ns Dx
