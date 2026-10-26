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


    void IThdObject::setTemperatureIntermediate(const double i_t)
    {
      d_temperatureIntermediate = i_t;
    }

    void IThdObject::applyTemperature()
    {
      if (!d_temperatureIntermediate)
        return;

      setTemperature(*d_temperatureIntermediate);

      // The next step must start from the actual temperature, which may be changed outside of the simulation
      d_temperatureIntermediate.reset();
    }

  } // ns thd
} // ns Dx
