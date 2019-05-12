#pragma once

#include "LaggyDxFwd.h"


class IResource
{
public:

  virtual ~IResource() = default;

  virtual void load(IRenderDevice& i_renderDevice) = 0;
  virtual void unload() = 0;

};
