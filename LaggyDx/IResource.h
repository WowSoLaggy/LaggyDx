#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IResource
  {
  public:

    virtual ~IResource() = default;

    virtual void load(IRenderDevice& i_renderDevice) = 0;
    virtual void unload() = 0;

  };

} // ns Dx
