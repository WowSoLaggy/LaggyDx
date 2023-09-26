#pragma once

#include "ActionType.h"
#include "KeyboardKeys.h"
#include "LaggyDxFwd.h"
#include "MouseKeys.h"

#include <LaggySdk/PairHash.h>


namespace Dx
{
  class ActionsMap
  {
  public:
    [[nodiscard]] bool isKeyBinded(KeyboardKey i_key, ActionType i_actionType) const;
    [[nodiscard]] bool isKeyBinded(MouseKey i_key, ActionType i_actionType) const;

    void setAction(KeyboardKey i_key, Action i_action, ActionType i_actionType);
    void setAction(MouseKey i_key, Action i_action, ActionType i_actionType);
    [[nodiscard]] const Action* getAction(KeyboardKey i_key, ActionType i_actionType) const;
    [[nodiscard]] const Action* getAction(MouseKey i_key, ActionType i_actionType) const;
  private:
    std::unordered_map<std::pair<KeyboardKey, ActionType>, Action, Sdk::PairHash> d_keyboardMap;
    std::unordered_map<std::pair<MouseKey, ActionType>, Action, Sdk::PairHash> d_mouseMap;
  };

} // ns Dx
