#pragma once

#include "VsResource.h"


namespace Dx
{
  class VsHlslResource : public VsResource
  {
  public:
    VsHlslResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
  };
} // ns Dx
