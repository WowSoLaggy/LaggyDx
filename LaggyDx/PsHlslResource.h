#pragma once

#include "PsResource.h"


namespace Dx
{
  class PsHlslResource : public PsResource
  {
  public:
    PsHlslResource(fs::path i_shaderFilePath);

    virtual void load() override;
  };
} // ns Dx
