#pragma once

#include "VsResource.h"


namespace Dx
{
  class VsHlslResource : public VsResource
  {
  public:
    VsHlslResource(fs::path i_shaderFilePath);

    virtual void load() override;
  };
} // ns Dx
