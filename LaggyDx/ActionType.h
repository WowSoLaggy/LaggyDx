#pragma once

#include <cstdint>


namespace Dx
{
  enum class ActionType : std::int32_t
  {
    OnPress = 0,
    OnRelease,
    Continuous,
  };

} // ns Dx
