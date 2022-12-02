#include "stdafx.h"
#include "MemoryTexture.h"


namespace Dx
{
  MemoryTexture::MemoryTexture(ID3D11ShaderResourceView* i_texture, const D3D11_TEXTURE2D_DESC& i_desc)
    : d_texture(i_texture)
    , d_textureDesc(std::make_shared<D3D11_TEXTURE2D_DESC>(i_desc))
  {
  }


  fs::path MemoryTexture::getFilename() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }

  const ImageDescription& MemoryTexture::getDescription() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }
  const Animations2Map& MemoryTexture::getAnimationsMap() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }

  bool MemoryTexture::checkAlpha(Sdk::Vector2I i_coords, int i_frame) const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }


  ID3D11ShaderResourceView* MemoryTexture::getTexturePtr() const
  {
    return d_texture;
  }

  const D3D11_TEXTURE2D_DESC& MemoryTexture::getTextureDesc() const
  {
    CONTRACT_EXPECT(d_textureDesc);
    return *d_textureDesc;
  }


  void MemoryTexture::load(IRenderDevice& i_renderDevice)
  {
  }

  void MemoryTexture::unload()
  {
  }

} // ns Dx
