#include "stdafx.h"
#include "InstanceBuffer.h"


namespace Dx
{
  InstanceBuffer::InstanceBuffer(const std::vector<InstancePosRotScale>& i_instances)
    : d_buffer(i_instances)
  {
  }


  int InstanceBuffer::getInstanceCount() const
  {
    return d_buffer.getVerticeCount();
  }

  const VertexBuffer& InstanceBuffer::getBuffer() const
  {
    return d_buffer;
  }

} // ns Dx
