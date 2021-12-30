#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IObject
  {
  public:
    virtual ~IObject() = default;

    virtual void update(double i_dt) = 0;
  };

} // ns Dx
