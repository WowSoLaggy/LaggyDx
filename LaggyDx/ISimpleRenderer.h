#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class ISimpleRenderer
  {
  public:
    static ISimpleRenderer& getOrCreate(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    virtual ~ISimpleRenderer() = default;

    virtual void draw(const IObject3& i_object) = 0;
  };

} // ns Dx
