#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/ISerializable.h>


namespace Dx
{
  namespace thd
  {
    class GasUnit : public Sdk::ISerializable
    {
    public:
      virtual void pushFields() override;
      virtual Sdk::FieldHandled onFieldNotFound(const std::string& i_name, const Json::Value& i_json) override;

      const GasesMap& getGases() const;
      [[nodiscard]] bool hasGas() const;
      [[nodiscard]] int getGasAmount() const;

      void setVolume(double i_volume);
      [[nodiscard]] double getVolume() const;

      [[nodiscard]] double getPressure() const;

      // Returns whether the unit contains gas but has no volume.
      // It may happen that gas has no volume but still has some amount (e.g. after the door is closed).
      // It leads to infinite pressure that shall be handled separately.
      [[nodiscard]] bool hasTrappedGas() const;

      // Returns true if gas has volume OR gas amount.
      // It may happen that gas has no volume but still has some amount (e.g. after the door is closed)
      [[nodiscard]] bool participateInGasSimulation() const;

      void clearGases();
      int addGas(GasId i_gasId, int i_amount); // returns actual amount added
      int removeGas(GasId i_gasId, int i_amount); // returns actual amount removed
      void addGases(const GasesMap& i_gases);
      void removeGases(const GasesMap& i_gases);
      GasesMap removeAmountOfGases(int i_amount);

    private:
      double d_volume = 1;
      int d_gasAmount = 0;
      GasesMap d_gases;
    };

  } // ns thd
} // ns Dx
