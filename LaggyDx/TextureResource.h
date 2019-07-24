#pragma once

#include "ILoadableResource.h"
#include "ImageAnnotation.h"
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

    Sdk::Vector2I getSize() const override;

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    bool d_loaded = false;
    const fs::path d_textureFilePath = "";

    ID3D11ShaderResourceView* d_texture = nullptr;
    ImageAnnotation d_annotation;

    void loadTexture(IRenderDevice& i_renderDevice);
    void setSize();
    void loadAnnotation();
  };

} // ns Dx
