#pragma once

#include "CollisionPointNormal.h"
#include "LaggyDxFwd.h"

namespace Dx
{
  struct CollisionInfo
  {
    const IObject2& sender;
    const IObject2& receiver;

    const CollisionPointNormal pointNormal;
  };

} // Dx
