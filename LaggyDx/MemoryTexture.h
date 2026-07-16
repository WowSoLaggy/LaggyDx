#pragma once

#include "ITexture.h"
#include "DxResourceWrapper.h"


namespace Dx
{
  class MemoryTexture : public ITexture
  {
  public:
    MemoryTexture(DxResourceWrapper<ID3D11Texture2D> i_texture, ID3D11ShaderResourceView& i_srv);
    virtual ~MemoryTexture() override;

    virtual fs::path getFilename() const override;

    virtual void load() override;
    virtual void unload() override;

    virtual const ImageDescription& getDescription() const override;
    virtual const Animations2Map& getAnimationsMap() const override;

    virtual bool hasAlpha() const override;
    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const override;

    virtual const std::shared_ptr<IBitmap> getBitmap() const override;

    virtual ID3D11ShaderResourceView* getTexturePtr() const override;
    virtual const D3D11_TEXTURE2D_DESC& getTextureDesc() const override;

  private:
    DxResourceWrapper<ID3D11Texture2D> d_texture;
    ID3D11ShaderResourceView& d_srv;
  };

} // ns Dx
