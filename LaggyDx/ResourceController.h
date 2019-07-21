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

    virtual const IMeshResourceCmo& getMeshResourceCmo(const std::string& i_resourceName) const override;
    virtual const ITextureResource& getTextureResource(const std::string& i_resourceName) const override;
    virtual const IPixelShaderResource& getPixelShaderResource(const std::string& i_resourceName) const override;
    virtual const IVertexShaderResource& getVertexShaderResource(const std::string& i_resourceName) const override;
    virtual const IFontResource& getFontResource(const std::string& i_resourceName) const override;

    virtual void loadResources(IRenderDevice& i_renderDevice) override;
    virtual void unloadResources() override;

  private:
    std::string d_resourceFolder;

    std::unordered_map<std::string, std::shared_ptr<MeshResourceCmo>> d_meshResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<TextureResource>> d_textureResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<PixelShaderResource>> d_pixelShaderResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<VertexShaderResource>> d_vertexShaderResourcesMap;
    std::unordered_map<std::string, std::shared_ptr<FontResource>> d_fontResourcesMap;

    void indexResourcesInDir(const std::string& i_dirName);
  };

} // ns Dx
