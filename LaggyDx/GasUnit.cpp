#include "stdafx.h"
#include "GasUnit.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  namespace thd
  {
    namespace
    {
      const std::string GasTagPreffix = "gas";
      const char GasTagSeparator = '.';

      std::string createGasTag(const GasId i_gasId)
      {
        return GasTagPreffix + GasTagSeparator + std::to_string(static_cast<int>(i_gasId));
      }

      bool isGasTag(const std::string& i_tag)
      {
        return i_tag.starts_with(GasTagPreffix + GasTagSeparator);
      }

      GasId parseGasId(const std::string& i_tag)
      {
        const auto tokens = Sdk::splitString(i_tag, GasTagSeparator);
        CONTRACT_EXPECT(tokens.size() == 2);

        const int gasIdInt = std::atoi(tokens[1].c_str());
        return static_cast<GasId>(gasIdInt);
      }

    } // anonym NS


    void GasUnit::pushFields()
    {
      pushField("volume", d_volume);
      pushField("gasAmount", d_gasAmount);

      for (const auto& [id, amount] : d_gases)
        pushField(createGasTag(id), amount);
    }

    Sdk::FieldHandled GasUnit::onFieldNotFound(const std::string& i_name, const Json::Value& i_json)
    {
      if (isGasTag(i_name))
      {
        const GasId gasId = parseGasId(i_name);
        d_gases[gasId] = i_json.asInt();

        return true;
      }

      return false;
    }


    const GasesMap& GasUnit::getGases() const
    {
      return d_gases;
    }

    bool GasUnit::hasGas() const
    {
      return d_gasAmount > 0;
    }

    int GasUnit::getGasAmount() const
    {
      return d_gasAmount;
    }


    void GasUnit::setVolume(const double i_volume)
    {
      d_volume = i_volume;
    }

    double GasUnit::getVolume() const
    {
      return d_volume;
    }


    double GasUnit::getPressure() const
    {
      return d_gasAmount / d_volume;
    }


    void GasUnit::clear()
    {
      d_gases.clear();
      d_gasAmount = 0;
    }

    void GasUnit::addGas(const GasId i_gasId, const int i_amount, bool i_allowNegative)
    {
      if (!i_allowNegative)
      {
        const int oldAmount = d_gases[i_gasId];
        d_gases[i_gasId] = std::max(0, d_gases[i_gasId] + i_amount);
        const int newAmount = d_gases[i_gasId];

        d_gasAmount += newAmount - oldAmount;
      }
      else
      {
        d_gases[i_gasId] += i_amount;
        d_gasAmount += i_amount;
      }
    }

    void GasUnit::addGases(const GasesMap& i_gases, bool i_allowNegative)
    {
      for (auto& [id, amount] : i_gases)
        addGas(id, amount, i_allowNegative);
    }

    void GasUnit::removeGases(const GasesMap& i_gases, bool i_allowNegative)
    {
      for (auto& [id, amount] : i_gases)
        addGas(id, -amount, i_allowNegative);
    }

    GasesMap GasUnit::calculateGasesPerRatio(const double i_ratio) const
    {
      GasesMap res;
      
      for (const auto& [id, amount] : d_gases)
      {
        const int amountToTake = (int)std::ceil(i_ratio * amount);
        res[id] += amountToTake;
      }

      return res;
    }

  } // ns thd
} // ns Dx
