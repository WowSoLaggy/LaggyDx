#include "stdafx.h"
#include "TextureView.h"


namespace Dx
{
  TextureView::TextureView(ID3D11ShaderResourceView* i_texture, const D3D11_TEXTURE2D_DESC& i_desc)
    : d_texture(i_texture)
    , d_textureDesc(std::make_shared<D3D11_TEXTURE2D_DESC>(i_desc))
  {
  }


  fs::path TextureView::getFilename() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }

  const ImageDescription& TextureView::getDescription() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }
  const Animations2Map& TextureView::getAnimationsMap() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }

  bool TextureView::hasAlpha() const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }
  bool TextureView::checkAlpha(Sdk::Vector2I i_coords, int i_frame) const
  {
    // Not implemented
    CONTRACT_ASSERT(false);
  }


  ID3D11ShaderResourceView* TextureView::getTexturePtr() const
  {
    return d_texture;
  }

  const D3D11_TEXTURE2D_DESC& TextureView::getTextureDesc() const
  {
    CONTRACT_EXPECT(d_textureDesc);
    return *d_textureDesc;
  }


  void TextureView::load(IRenderDevice& i_renderDevice)
  {
  }

  void TextureView::unload()
  {
  }

} // ns Dx
