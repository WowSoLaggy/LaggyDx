#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


TextureResource::TextureResource(std::string i_textureFilePath)
  : d_textureFilePath(std::move(i_textureFilePath))
{
}


void TextureResource::load(IRenderDevice& i_renderDevice)
{
  auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

  CreateDDSTextureFromFile(renderDevice.getDevicePtr(),
                           Sdk::getWString(d_textureFilePath).c_str(), nullptr, &d_texture);
}

void TextureResource::unload()
{
  d_texture->Release();
}
