#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Sprite
  {
    const ITextureResource* texture = nullptr;

    Sdk::Vector2 position = Sdk::Vector2::zero();
    Sdk::Vector2 size = Sdk::Vector2::zero();
  };

} // ns Dx
