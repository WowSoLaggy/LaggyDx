#pragma once

#include "ShaderBase.h"


namespace Dx
{
  class Shader3d : public ShaderBase
  {
  public:
    virtual void draw(const IObject3& i_object) const = 0;

  };

} // ns Dx
