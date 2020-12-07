#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class ILoadableResource
  {
  public:
    virtual ~ILoadableResource() = default;

    virtual void loadIfNeeded(IRenderDevice& i_renderDevice) = 0;
    virtual void unloadIfNeeded() = 0;
  };
} // ns Dx
