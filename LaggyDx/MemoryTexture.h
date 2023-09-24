#pragma once

#include "TextureBase.h"


namespace Dx
{
  // Memory texture DOESN'T own texture ptr!
  class MemoryTexture : public TextureBase
  {
  public:
    MemoryTexture(ID3D11ShaderResourceView* i_texture, const D3D11_TEXTURE2D_DESC& i_desc);

    virtual fs::path getFilename() const override;

    virtual const ImageDescription& getDescription() const override;
    virtual const Animations2Map& getAnimationsMap() const override;

    virtual bool hasAlpha() const override;
    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const override;

    virtual ID3D11ShaderResourceView* getTexturePtr() const override;
    virtual const D3D11_TEXTURE2D_DESC& getTextureDesc() const override;

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

  private:
    ID3D11ShaderResourceView* d_texture = nullptr;
    std::shared_ptr<D3D11_TEXTURE2D_DESC> d_textureDesc;
  };

} // ns Dx
