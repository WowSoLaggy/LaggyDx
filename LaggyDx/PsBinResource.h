#pragma once

#include "PsResource.h"


namespace Dx
{
  class PsBinResource : public PsResource
  {
  public:
    PsBinResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
  };
} // ns Dx
