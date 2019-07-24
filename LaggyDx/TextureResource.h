#pragma once

#include "ILoadableResource.h"
#include "ITextureResource.h"


struct ID3D11ShaderResourceView;


namespace Dx
{
  class TextureResource : public ILoadableResource, public ITextureResource
  {
  public:
    TextureResource(fs::path i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    const Sdk::Vector2I& getSize() const override { return d_size; }

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    bool d_loaded = false;
    const fs::path d_textureFilePath = "";
    Sdk::Vector2I d_size{ 0, 0 };

    ID3D11ShaderResourceView* d_texture = nullptr;

    void loadTexture(IRenderDevice& i_renderDevice);
    void setSize();
    void loadAnnotation();
  };

} // ns Dx
