#include "stdafx.h"
#include "FontResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


FontResource::FontResource(std::string i_fontFilePath)
  : d_fontFilePath(std::move(i_fontFilePath))
{
}


void FontResource::load(IRenderDevice& i_renderDevice)
{
  auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

  d_spriteFont = std::make_shared<SpriteFont>(renderDevice.getDevicePtr(), Sdk::getWString(d_fontFilePath).c_str());
}

void FontResource::unload()
{
  d_spriteFont.reset();
}
