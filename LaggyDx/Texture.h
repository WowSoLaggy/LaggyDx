#pragma once

#include "ImageAnimation.h"
#include "ImageDescription.h"
#include "TextureBase.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class Texture : public TextureBase
  {
  public:
    Texture(fs::path i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual fs::path getFilename() const override;

    virtual const ImageDescription& getDescription() const override;
    virtual const Animations2Map& getAnimationsMap() const override;

    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const override;

    virtual ID3D11ShaderResourceView* getTexturePtr() const override;
    virtual const D3D11_TEXTURE2D_DESC& getTextureDesc() const override;

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
