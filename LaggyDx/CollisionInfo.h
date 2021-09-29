#pragma once

#include "CollisionPointNormal.h"
#include "LaggyDxFwd.h"

namespace Dx
{
  struct CollisionInfo
  {
    const IObject& sender;
    const IObject& receiver;

    const CollisionPointNormal pointNormal;
  };

} // Dx
