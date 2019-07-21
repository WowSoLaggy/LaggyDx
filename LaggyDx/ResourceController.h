#pragma once

#include "IResourceController.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ResourceController : public IResourceController
  {
  public:
    ResourceController(const std::string& i_resourcesFolder);
    virtual ~ResourceController() override;

    virtual ResourceId getResourceId(const std::string& i_resourceName) const override;

    const MeshResourceCmo& getMeshResourceCmo(ResourceId i_resourceId) const;
    const TextureResource& getTextureResource(ResourceId i_resourceId) const;
    const PixelShaderResource& getPixelShaderResource(ResourceId i_resourceId) const;
    const VertexShaderResource& getVertexShaderResource(ResourceId i_resourceId) const;
    const FontResource& getFontResource(ResourceId i_resourceId) const;

    virtual void loadResources(IRenderDevice& i_renderDevice) override;
    virtual void unloadResources() override;

  private:
    std::string d_resourceFolder;

    std::unordered_map<std::string, ResourceId> d_nameToIdMap;
    std::unordered_map<ResourceId, std::shared_ptr<ILoadableResource>> d_idToResourceMap;

    ResourceId d_nextResourceId;
    ResourceId getFreeResourceId();

    void indexResourcesInDir(const std::string& i_dirName);
  };

} // ns Dx
