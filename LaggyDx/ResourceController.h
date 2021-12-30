#pragma once

#include "IResourceController.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  using ResourcesMap = std::unordered_map<std::string, std::shared_ptr<ILoadableResource>>;

  class ResourceController : public IResourceController
  {
  public:
    ResourceController(IRenderDevice& i_renderDevice, fs::path i_resourcesFolder);
    virtual ~ResourceController() override;

    virtual const IMeshResourceCmo& getMeshResourceCmo(const std::string& i_resourceName) const override;
    virtual const ITextureResource& getTextureResource(const std::string& i_resourceName) const override;
    virtual const IPixelShaderResource& getPixelShaderResource(const std::string& i_resourceName) const override;
    virtual const IVertexShaderResource& getVertexShaderResource(const std::string& i_resourceName) const override;
    virtual const IFontResource& getFontResource(const std::string& i_resourceName) const override;

    virtual void loadResources() const override;
    virtual void unloadResources() const override;

  private:
    IRenderDevice& d_renderDevice;

    fs::path d_resourceFolder;

    ResourcesMap d_resources;

    void loadResource(ILoadableResource& i_resource) const;
    void indexResourcesInDir(const fs::path& i_dirName);
  };

} // ns Dx
