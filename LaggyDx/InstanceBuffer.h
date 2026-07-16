#pragma once

#include "IInstanceBuffer.h"
#include "VertexBuffer.h"


namespace Dx
{
  class InstanceBuffer : public IInstanceBuffer
  {
  public:
    InstanceBuffer(const std::vector<InstancePosRotScale>& i_instances);

    virtual int getInstanceCount() const override;

    const VertexBuffer& getBuffer() const;

  private:
    VertexBuffer d_buffer;
  };

} // ns Dx
