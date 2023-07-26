#pragma once

#include "ShaderBase.h"


namespace Dx
{
  class Shader2d : public ShaderBase
  {
  public:
    virtual void draw() const = 0;

  };

} // ns Dx
