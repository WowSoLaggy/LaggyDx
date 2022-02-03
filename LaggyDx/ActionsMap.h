#pragma once

#include "Action.h"
#include "ActionType.h"
#include "KeyboardKeys.h"
#include "MouseKeys.h"

#include <LaggySdk/PairHash.h>


namespace Dx
{
  class ActionsMap
  {
  public:
    [[nodiscard]] bool isKeyBinded(Dx::KeyboardKey i_key, ActionType i_actionType) const;
    [[nodiscard]] bool isKeyBinded(Dx::MouseKey i_key, ActionType i_actionType) const;

    void setAction(Dx::KeyboardKey i_key, Action i_action, ActionType i_actionType);
    void setAction(Dx::MouseKey i_key, Action i_action, ActionType i_actionType);
    [[nodiscard]] const Action* getAction(Dx::KeyboardKey i_key, ActionType i_actionType) const;
    [[nodiscard]] const Action* getAction(Dx::MouseKey i_key, ActionType i_actionType) const;
  private:
    std::unordered_map<std::pair<Dx::KeyboardKey, ActionType>, Action, Sdk::PairHash> d_keyboardMap;
    std::unordered_map<std::pair<Dx::MouseKey, ActionType>, Action, Sdk::PairHash> d_mouseMap;
  };

} // ns Dx
