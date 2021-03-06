#pragma once

#include "ImageAnimation.h"
#include "ImageDescription.h"
#include "ITextureResource.h"
#include "LoadableResource.h"

#include <unordered_map>


struct ID3D11ShaderResourceView;


namespace Dx
{
  class TextureResource : public LoadableResource, public ITextureResource
  {
  public:
    TextureResource(fs::path i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual fs::path getFilename() const override;

    virtual const ImageDescription& getDescription() const override { return d_description; }
    virtual const AnimationsMap& getAnimationsMap() const override { return d_animations; }

    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const override;

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    const fs::path d_textureFilePath = "";

    ID3D11ShaderResourceView* d_texture = nullptr;
    D3D11_TEXTURE2D_DESC d_textureDesc;
    ImageDescription d_description;
    AnimationsMap d_animations;

    bool d_solidAlpha = true;
    std::vector<bool> d_alphaMask;

    void loadTexture(IRenderDevice& i_renderDevice);
    void setSizeFromTexture();
    void loadAnnotation();
    void fillAlphaMask(IRenderDevice& i_renderDevice);
  };
} // ns Dx
