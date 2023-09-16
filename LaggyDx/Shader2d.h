#pragma once

#include "LaggyDxFwd.h"
#include "ShaderBase.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct UvOffset
  {
    Sdk::Vector2F offset = Sdk::Vector2F::zero();
    Sdk::Vector2F multiplier = Sdk::Vector2F::identity();
  };

  class Shader2d : public ShaderBase
  {
  public:
    virtual void draw(
      const ISprite& i_sprite,
      const UvOffset* i_uvOffset = nullptr,
      bool i_disableCameraView = false) const = 0;

  };

} // ns Dx
