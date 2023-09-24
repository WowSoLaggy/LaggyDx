#include "stdafx.h"
#include "CursorUtils.h"

#include "App.h"


namespace Dx
{
  const Sdk::Vector2I& CursorUtils::getPosition()
  {
    return App::get().getInputDevice().getMousePosition();
  }

} // ns Dx
