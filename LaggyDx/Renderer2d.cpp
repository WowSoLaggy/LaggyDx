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


  void Renderer2d::setTranslation(Sdk::Vector2 i_translation)
  {
    d_translation = std::move(i_translation);
  }

  void Renderer2d::resetTranslation()
  {
    d_translation = { 0, 0 };
  }


  void Renderer2d::renderText(const std::string& i_text,
    const IFontResource& i_fontResource, const Sdk::Vector2& i_position)
  {
    const auto& fontResource = dynamic_cast<const FontResource&>(i_fontResource);

    fontResource.getSpriteFont()->DrawString(d_spriteBatch.get(), Sdk::getWString(i_text).c_str(),
      XMFLOAT2(i_position.x, i_position.y));
  }


  void Renderer2d::renderTexture(const ITextureResource& i_textureResource, const Sdk::Vector2& i_position)
  {
    renderTexture(i_textureResource, i_position, i_textureResource.getSize());
  }

  void Renderer2d::renderTexture(const ITextureResource& i_textureResource,
    const Sdk::Vector2& i_position, const Sdk::Vector2& i_size)
  {
    const auto& textureResource = dynamic_cast<const TextureResource&>(i_textureResource);

    const auto pos = i_position - d_translation;

    RECT destinationRect{ (int)pos.x, (int)pos.y,
      (int)(pos.x + i_size.x), (int)(pos.y + i_size.y) };
    d_spriteBatch->Draw(textureResource.getTexturePtr(), destinationRect, Colors::White);
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    if (i_sprite.texture)
      renderTexture(*i_sprite.texture, i_sprite.position, i_sprite.size);
  }

} // ns Dx
