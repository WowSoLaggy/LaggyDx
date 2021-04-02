#pragma once

#include "KeyboardKeys.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  std::unordered_set<KeyboardKey> getKeys(const KeyboardKeysState& i_state);

} // ns Dx
