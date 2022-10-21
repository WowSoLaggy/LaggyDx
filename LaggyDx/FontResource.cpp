#include "stdafx.h"
#include "FontResource.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/StringUtils.h>


namespace Dx
{
  FontResource::FontResource(fs::path i_fontFilePath)
    : d_fontFilePath(std::move(i_fontFilePath))
  {
  }


  void FontResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    d_spriteFont = std::make_shared<SpriteFont>(renderDevice.getDevicePtr(), d_fontFilePath.wstring().c_str());
  }

  void FontResource::unload()
  {
    d_spriteFont.reset();
  }


  Sdk::RectI FontResource::getStringRect(const std::string& i_string) const
  {
    CONTRACT_EXPECT(d_spriteFont);

    const auto stringSize = d_spriteFont->MeasureString(Sdk::s2ws(i_string).c_str());
    return { Sdk::Vector2I::zero(), { (int)XMVectorGetX(stringSize), (int)XMVectorGetY(stringSize) } };
  }
} // ns Dx
