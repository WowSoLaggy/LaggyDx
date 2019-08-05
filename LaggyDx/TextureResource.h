#pragma once

#include "ILoadableResource.h"
#include "ImageAnimation.h"
#include "ImageDescription.h"
#include "ITextureResource.h"

#include <unordered_map>


struct ID3D11ShaderResourceView;


namespace Dx
{
  class TextureResource : public ILoadableResource, public ITextureResource
  {
  public:
    TextureResource(fs::path i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual bool isLoaded() const override { return d_loaded; }

    virtual const ImageDescription& getDescription() const override { return d_description; }
    virtual const AnimationsMap& getAnimationsMap() const override { return d_animations; }

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    bool d_loaded = false;
    const fs::path d_textureFilePath = "";

    ID3D11ShaderResourceView* d_texture = nullptr;
    ImageDescription d_description;
    AnimationsMap d_animations;

    void loadTexture(IRenderDevice& i_renderDevice);
    void setSize();
    void loadAnnotation();
  };
} // ns Dx
