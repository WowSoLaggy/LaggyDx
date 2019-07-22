#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Sprite
  {
    const ITextureResource* texture = nullptr;

    Sdk::Vector2I position = Sdk::Vector2I::zero();
    Sdk::Vector2I size = Sdk::Vector2I::zero();
  };

} // ns Dx
