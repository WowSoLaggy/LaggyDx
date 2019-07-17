#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IResourceController
  {
  public:
    static std::unique_ptr<IResourceController> create(const std::string& i_resourcesFolder);

  public:
    virtual ~IResourceController() = default;

    virtual ResourceId getResourceId(const std::string& i_resourceName) const = 0;

    virtual void loadResources(IRenderDevice& i_renderDevice) = 0;
    virtual void unloadResources() = 0;
  };

} // ns Dx
