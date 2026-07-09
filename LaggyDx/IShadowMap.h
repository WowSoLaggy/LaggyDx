#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  // Depth-only offscreen render target for shadow mapping
  class IShadowMap
  {
  public:
    static std::unique_ptr<IShadowMap> create(IRenderDevice& i_renderDevice, int i_resolution);

  public:
    virtual ~IShadowMap() = default;

    virtual int getResolution() const = 0;
    virtual const ITexture& getTexture() const = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
  };

} // ns Dx
