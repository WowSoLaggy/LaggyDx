#pragma once

#include "ImageAnimation.h"
#include "ImageDescription.h"
#include "LaggyDxFwd.h"
#include "ITextureResource.h"


namespace Dx
{
  class TextureResource : public ITextureResource
  {
  public:
    TextureResource(fs::path i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual fs::path getFilename() const override;

    virtual const ImageDescription& getDescription() const override { return d_description; }
    virtual const Animations2Map& getAnimationsMap() const override { return d_animations; }

    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const override;

    virtual const std::shared_ptr<IBitmap> getBitmap(IRenderDevice& i_renderDevice) const override;

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    const fs::path d_textureFilePath = "";

    ID3D11ShaderResourceView* d_texture = nullptr;
    D3D11_TEXTURE2D_DESC d_textureDesc = {};
    ImageDescription d_description;
    Animations2Map d_animations;

    bool d_solidAlpha = true;
    std::vector<bool> d_alphaMask;

    void loadTexture(IRenderDevice& i_renderDevice);
    void setSizeFromTexture();
    void loadAnnotation();
    void fillAlphaMask(IRenderDevice& i_renderDevice);
  };
} // ns Dx
