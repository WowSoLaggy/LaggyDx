#pragma once

#include "PsResource.h"


namespace Dx
{
  class PsHlslResource : public PsResource
  {
  public:
    PsHlslResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
  };
} // ns Dx
