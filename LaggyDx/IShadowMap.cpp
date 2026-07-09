#include "stdafx.h"
#include "IShadowMap.h"

#include "ShadowMap.h"


namespace Dx
{
  std::unique_ptr<IShadowMap> IShadowMap::create(IRenderDevice& i_renderDevice, const int i_resolution)
  {
    CONTRACT_EXPECT(i_resolution > 0);
    return std::make_unique<ShadowMap>(i_renderDevice, i_resolution);
  }

} // ns Dx
