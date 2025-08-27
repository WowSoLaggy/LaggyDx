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

    virtual bool hasResource(const fs::path& i_name) const override;

    virtual const IMeshResourceCmo& getMeshCmo(const fs::path& i_name) const override;
    virtual const ITexture& getTexture(const fs::path& i_name) const override;
    virtual const IPsResource& getPs(const fs::path& i_name) const override;
    virtual const IVsResource& getVs(const fs::path& i_name) const override;
    virtual const IFontResource& getFont(const fs::path& i_name) const override;
    virtual const IFbxResource& getFbx(const fs::path& i_name) const override;
    virtual const IGltfResource& getGltf(const fs::path& i_name) const override;

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
