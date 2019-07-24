#include "stdafx.h"
#include "Renderer2d.h"

#include "FontResource.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "Sprite.h"
#include "TextureResource.h"

#include <LaggySdk/StringUtils.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  Renderer2d::Renderer2d(IRenderDevice& io_renderDevice)
    : d_renderDevice(io_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);

    d_spriteBatch = std::make_shared<SpriteBatch>(renderDevice.getDeviceContextPtr());
    d_states = std::make_unique<CommonStates>(renderDevice.getDevicePtr());
  }


  void Renderer2d::beginScene()
  {
    d_spriteBatch->Begin(SpriteSortMode_Deferred, d_states->NonPremultiplied());
  }

  void Renderer2d::endScene()
  {
    d_spriteBatch->End();
  }


  void Renderer2d::setTranslation(Sdk::Vector2I i_translation)
  {
    d_translation = std::move(i_translation);
  }

  void Renderer2d::resetTranslation()
  {
    d_translation = { 0, 0 };
  }


  void Renderer2d::renderText(const std::string& i_text,
    const IFontResource& i_fontResource, const Sdk::Vector2I& i_position)
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    fontResource.getSpriteFont()->DrawString(d_spriteBatch.get(), Sdk::getWString(i_text).c_str(),
      XMFLOAT2((float)i_position.x, (float)i_position.y));
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    if (!i_sprite.texture)
      return;

    const auto& textureResource = dynamic_cast<const TextureResource&>(*i_sprite.texture);

    const auto pos = i_sprite.position - d_translation;

    RECT destinationRect{ pos.x, pos.y, pos.x + i_sprite.size.x, pos.y + i_sprite.size.y };
    d_spriteBatch->Draw(textureResource.getTexturePtr(), destinationRect, Colors::White);
  }

} // ns Dx
