#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IResourceController
  {
  public:
    static std::unique_ptr<IResourceController> create(IRenderDevice& i_renderDevice,
                                                       fs::path i_resourcesFolder);

  public:
    virtual ~IResourceController() = default;

    virtual const IMeshResourceCmo& getMeshResourceCmo(const std::string& i_resourceName) const = 0;
    virtual const ITextureResource& getTextureResource(const std::string& i_resourceName) const = 0;
    virtual const IPsResource& getPsResource(const std::string& i_resourceName) const = 0;
    virtual const IVsResource& getVsResource(const std::string& i_resourceName) const = 0;
    virtual const IFontResource& getFontResource(const std::string& i_resourceName) const = 0;
    virtual const IFbxResource& getFbxResource(const std::string& i_resourceName) const = 0;

    virtual void loadResources() const = 0;
    virtual void unloadResources() const = 0;
  };

} // ns Dx
