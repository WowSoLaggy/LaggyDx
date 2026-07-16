#include "stdafx.h"
#include "IInstanceBuffer.h"

#include "InstanceBuffer.h"


namespace Dx
{
  std::unique_ptr<IInstanceBuffer> IInstanceBuffer::create(const std::vector<InstancePosRotScale>& i_instances)
  {
    CONTRACT_EXPECT(!i_instances.empty());
    return std::make_unique<InstanceBuffer>(i_instances);
  }

} // ns Dx
