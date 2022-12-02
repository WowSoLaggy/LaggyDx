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

    virtual bool hasResource(const fs::path& i_name) const = 0;

    virtual const IMeshResourceCmo& getMeshCmo(const fs::path& i_name) const = 0;
    virtual const ITexture& getTexture(const fs::path& i_name) const = 0;
    virtual const IPsResource& getPs(const fs::path& i_name) const = 0;
    virtual const IVsResource& getVs(const fs::path& i_name) const = 0;
    virtual const IFontResource& getFont(const fs::path& i_name) const = 0;
    virtual const IFbxResource& getFbx(const fs::path& i_name) const = 0;

    virtual void loadResources() const = 0;
    virtual void unloadResources() const = 0;
  };

} // ns Dx
