#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class ISimpleRenderer
  {
  public:
    static ISimpleRenderer& getOrCreate(
      IRenderDevice& i_renderDevice,
      const ICamera3& i_camera,
      const IResourceController& i_resourceController);

  public:
    virtual ~ISimpleRenderer() = default;

    virtual void setDrawAabb(bool i_draw) = 0;

    virtual void draw(const IObject3& i_object) const = 0;
  };

} // ns Dx
