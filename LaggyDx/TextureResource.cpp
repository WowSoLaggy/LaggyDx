#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/StringUtils.h>


namespace Dx
{
  TextureResource::TextureResource(std::string i_textureFilePath)
    : d_textureFilePath(std::move(i_textureFilePath))
    , d_loaded(false)
  {
  }


  void TextureResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    CreateDDSTextureFromFile(renderDevice.getDevicePtr(),
      Sdk::getWString(d_textureFilePath).c_str(), nullptr, &d_texture);
    CONTRACT_EXPECT(d_texture);

    ID3D11Texture2D* textureResource = nullptr;
    d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&textureResource));
    D3D11_TEXTURE2D_DESC desc;
    textureResource->GetDesc(&desc);

    d_size = { (float)desc.Width, (float)desc.Height };

    d_loaded = true;
  }

  void TextureResource::unload()
  {
    if (d_texture && d_loaded)
      d_texture->Release();

    d_loaded = false;
  }

} // ns Dx
