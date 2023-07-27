#pragma once

#include "LaggyDxFwd.h"
#include "ShaderBase.h"


namespace Dx
{
  class Shader2d : public ShaderBase
  {
  public:
    virtual void draw(const ISprite& i_sprite) const = 0;

  };

} // ns Dx
