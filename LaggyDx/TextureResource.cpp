#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  TextureResource::TextureResource(std::string i_textureFilePath)
    : d_textureFilePath(std::move(i_textureFilePath))
  {
  }


  void TextureResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    CreateDDSTextureFromFile(renderDevice.getDevicePtr(),
      Sdk::getWString(d_textureFilePath).c_str(), nullptr, &d_texture);

    ID3D11Texture2D* textureResource = nullptr;
    d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&textureResource));
    D3D11_TEXTURE2D_DESC desc;
    textureResource->GetDesc(&desc);

    d_size = { (float)desc.Width, (float)desc.Height };
  }

  void TextureResource::unload()
  {
    d_texture->Release();
  }

} // ns Dx
