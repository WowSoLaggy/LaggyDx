#pragma once

#include "Shader2d.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ISpriteShader : public Shader2d
  {
  public:
    static std::unique_ptr<ISpriteShader> create();
  };

} // ns Dx
