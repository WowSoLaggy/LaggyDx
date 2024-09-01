#pragma once


namespace Dx
{
  namespace thd
  {
    class IThdObject
    {
    public:
      virtual ~IThdObject() = default;

      // Optional because tiles may have no temperature (e.g. vacuum)
      virtual std::optional<double> getTemperature() const = 0;
      virtual void setTemperature(double i_t) = 0;

      virtual double getThermalConductivity() const = 0;
      virtual double getHeatCapacity() const = 0;
    };

  } // ns thd
} // ns Dx
