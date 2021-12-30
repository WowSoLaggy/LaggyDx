#pragma once

#include "VsResource.h"


namespace Dx
{
  class VsBinResource : public VsResource
  {
  public:
    VsBinResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
  };
} // ns Dx
