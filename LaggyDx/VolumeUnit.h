#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/ISerializable.h>


namespace Dx
{
  namespace thd
  {
    class VolumeUnit : public Sdk::ISerializable
    {
    public:
      virtual void pushFields() override;

      void setVolume(double i_volume);
      double getVolume() const;

      double getPressure() const;

      void addGas(GasId i_gasId, int i_count);

    private:
      double d_volume = 1;
      GasesMap d_gases;
    };

  } // ns thd
} // ns Dx
