#include "stdafx.h"
#include "ActionsMap.h"


namespace Dx
{
  bool ActionsMap::isKeyBinded(const KeyboardKey i_key, ActionType i_actionType) const
  {
    const auto it = d_keyboardMap.find(std::make_pair(i_key, i_actionType));
    return it != d_keyboardMap.cend();
  }

  bool ActionsMap::isKeyBinded(const MouseKey i_key, ActionType i_actionType) const
  {
    const auto it = d_mouseMap.find(std::make_pair(i_key, i_actionType));
    return it != d_mouseMap.cend();
  }


  void ActionsMap::setAction(const KeyboardKey i_key, Action i_action, const ActionType i_actionType)
  {
    d_keyboardMap[std::make_pair(i_key, i_actionType)] = std::move(i_action);
  }

  void ActionsMap::setAction(const MouseKey i_key, Action i_action, ActionType i_actionType)
  {
    d_mouseMap[std::make_pair(i_key, i_actionType)] = std::move(i_action);
  }

  const Action* ActionsMap::getAction(const KeyboardKey i_key, const ActionType i_actionType) const
  {
    const auto it = d_keyboardMap.find(std::make_pair(i_key, i_actionType));
    if (it == d_keyboardMap.cend())
      return nullptr;

    return &it->second;
  }

  const Action* ActionsMap::getAction(const MouseKey i_key, ActionType i_actionType) const
  {
    const auto it = d_mouseMap.find(std::make_pair(i_key, i_actionType));
    if (it == d_mouseMap.cend())
      return nullptr;

    return &it->second;
  }

} // ns Dx
