#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  namespace thd
  {
    class IThdObject
    {
    public:
      virtual ~IThdObject() = default;

      [[nodiscard]] virtual double getTemperature() const = 0;
      virtual void setTemperature(double i_t) = 0;
      [[nodiscard]] virtual double getThermalConductivity() const = 0;
      [[nodiscard]] virtual double getHeatCapacity() const = 0;

      [[nodiscard]] bool isGas() const;
      // Can be nullptr if the object is not gas
      [[nodiscard]] virtual GasUnit* getGasUnit();
      [[nodiscard]] virtual const GasUnit* getGasUnit() const;

      void transferHeat(double i_heatAmount);
      void applyTemperature();

    private:
      // This is a helper variable to store the temperature calculated in applyTemperature().
      std::optional<double> d_temperatureIntermediate;
    };

  } // ns thd
} // ns Dx
