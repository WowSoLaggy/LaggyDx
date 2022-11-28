#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IShader
  {
  public:
    virtual ~IShader() = default;

    virtual void draw(const IObject3& i_object) const = 0;
    
    virtual bool getFillMode() const = 0;
    virtual void setFillMode(bool i_fill) = 0;
  };

} // ns Dx
