#include "stdafx.h"
#include "TextRenderer.h"

#include "App.h"
#include "FontResource.h"
#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  TextRenderer::TextRenderer()
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(App::get().getRenderDevice());
    d_spriteBatch = std::make_unique<SpriteBatch>(renderDevice.getDeviceContextPtr());
    d_states = std::make_unique<CommonStates>(renderDevice.getDevicePtr());
  }


  void TextRenderer::beginScene()
  {
    d_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, d_states->NonPremultiplied());
  }
  void TextRenderer::endScene()
  {
    d_spriteBatch->End();
  }


  void TextRenderer::render(
    const std::string& i_text,
    const IFontResource& i_fontResource,
    const Sdk::Vector2F& i_translation,
    const Sdk::Vector4F& i_color,
    float i_scale) const
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    fontResource.getSpriteFont()->DrawString(
      d_spriteBatch.get(),
      Sdk::s2ws(i_text).c_str(),
      XMFLOAT2(i_translation.x, i_translation.y),
      { i_color.x, i_color.y, i_color.z, i_color.w },
      0,
      { 0, 0 },
      i_scale);
  }

} // ns Dx
