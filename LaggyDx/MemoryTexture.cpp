#include "stdafx.h"
#include "MemoryTexture.h"


namespace Dx
{
  MemoryTexture::MemoryTexture(DxResourceWrapper<ID3D11Texture2D> i_texture, ID3D11ShaderResourceView& i_srv)
    : d_texture(std::move(i_texture))
    , d_srv(i_srv)
  {
  }

  MemoryTexture::~MemoryTexture()
  {
    d_srv.Release();
  }


  fs::path MemoryTexture::getFilename() const
  {
    return fs::path();
  }


  void MemoryTexture::load(IRenderDevice& i_renderDevice)
  {
    // Managed by the owner, no-op
  }

  void MemoryTexture::unload()
  {
    // Managed by the owner, no-op
  }


  const ImageDescription& MemoryTexture::getDescription() const
  {
    CONTRACT_THROW("Not Implemented");
  }
  const Animations2Map& MemoryTexture::getAnimationsMap() const
  {
    CONTRACT_THROW("Not Implemented");
  }

  bool MemoryTexture::hasAlpha() const
  {
    CONTRACT_THROW("Not Implemented");
  }
  bool MemoryTexture::checkAlpha(Sdk::Vector2I i_coords, const int i_frame) const
  {
    CONTRACT_THROW("Not Implemented");
  }

  const std::shared_ptr<IBitmap> MemoryTexture::getBitmap(IRenderDevice& i_renderDevice) const
  {
    CONTRACT_THROW("Not Implemented");
  }


  ID3D11ShaderResourceView* MemoryTexture::getTexturePtr() const
  {
    return &d_srv;
  }

  const D3D11_TEXTURE2D_DESC& MemoryTexture::getTextureDesc() const
  {
    CONTRACT_THROW("Not Implemented");
  }

} // ns Dx
