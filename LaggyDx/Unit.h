#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/ISerializable.h>


namespace Dx
{
  namespace thd
  {
    class Unit : public Sdk::ISerializable
    {
    public:
      virtual void pushFields() override;
      virtual Sdk::FieldHandled onFieldNotFound(const std::string& i_name, const Json::Value& i_json) override;

      const GasesMap& getGases() const;
      bool hasGas() const;
      int getGasAmount() const;

      void setVolume(double i_volume);
      double getVolume() const;

      double getPressure() const;

      void clear();
      void addGas(GasId i_gasId, int i_amount, bool i_allowNegative = false);
      void addGases(const GasesMap& i_gases, bool i_allowNegative = false);
      void removeGases(const GasesMap& i_gases, bool i_allowNegative = false);
      GasesMap extractGases(double i_ratio) const;

    private:
      double d_volume = 1;
      int d_gasAmount = 0;
      GasesMap d_gases;
    };

  } // ns thd
} // ns Dx
