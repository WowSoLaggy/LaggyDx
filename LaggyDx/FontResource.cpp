#include "stdafx.h"
#include "FontResource.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/StringUtils.h>


namespace Dx
{
  FontResource::FontResource(std::string i_fontFilePath)
    : d_fontFilePath(std::move(i_fontFilePath))
  {
  }


  void FontResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    d_spriteFont = std::make_shared<SpriteFont>(renderDevice.getDevicePtr(), Sdk::getWString(d_fontFilePath).c_str());

    d_loaded = true;
  }

  void FontResource::unload()
  {
    d_loaded = false;
    d_spriteFont.reset();
  }


  Sdk::RectI FontResource::getStringRect(const std::string& i_string) const
  {
    CONTRACT_EXPECT(d_spriteFont);

    const auto stringSize = d_spriteFont->MeasureString(Sdk::getWString(i_string).c_str());
    return { Sdk::Vector2I::zero(), { (int)XMVectorGetX(stringSize), (int)XMVectorGetY(stringSize) } };
  }
} // ns Dx
