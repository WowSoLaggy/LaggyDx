#pragma once

#include "InstanceTypes.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  // GPU buffer of per-instance transforms consumed by IInstancedShader.
  class IInstanceBuffer
  {
  public:
    static std::unique_ptr<IInstanceBuffer> create(
      const IRenderDevice& i_renderDevice, const std::vector<InstancePosRotScale>& i_instances);

  public:
    virtual ~IInstanceBuffer() = default;

    virtual int getInstanceCount() const = 0;
  };

} // ns Dx
