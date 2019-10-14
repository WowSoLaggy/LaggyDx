#pragma once

#include "IResourceController.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ResourceController : public IResourceController
  {
  public:
    ResourceController(IRenderDevice& i_renderDevice, const std::string& i_resourcesFolder);
    virtual ~ResourceController() override;

    virtual const IMeshResourceCmo& getMeshResourceCmo(const std::string& i_resourceName) override;
    virtual const ITextureResource& getTextureResource(const std::string& i_resourceName) override;
    virtual const IPixelShaderResource& getPixelShaderResource(const std::string& i_resourceName) override;
    virtual const IVertexShaderResource& getVertexShaderResource(const std::string& i_resourceName) override;
    virtual const IFontResource& getFontResource(const std::string& i_resourceName) override;

    virtual void loadResources() override;
    virtual void unloadResources() override;

  private:
    IRenderDevice& d_renderDevice;

    std::string d_resourceFolder;

    std::unordered_map<std::string, std::shared_ptr<MeshResourceCmo>> d_meshResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<TextureResource>> d_textureResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<PixelShaderResource>> d_pixelShaderResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<VertexShaderResource>> d_vertexShaderResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<FontResource>> d_fontResourcesMap;

    void loadResource(ILoadableResource& i_resource);
    void indexResourcesInDir(const std::string& i_dirName);
  };

} // ns Dx
