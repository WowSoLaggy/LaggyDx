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
  Renderer2d::Renderer2d(
    IRenderDevice& io_renderDevice,
    const IResourceController& i_resourceController)
    : d_renderDevice(io_renderDevice)
    , d_resourceController(i_resourceController)
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


  void Renderer2d::renderText(const std::string& i_text, ResourceId i_fontResourceId, const Sdk::Vector2& i_position)
  {
    const auto& resourceController = dynamic_cast<const ResourceController&>(d_resourceController);
    const auto& fontResource = resourceController.getFontResource(i_fontResourceId);

    fontResource.getSpriteFont()->DrawString(d_spriteBatch.get(), Sdk::getWString(i_text).c_str(),
      XMFLOAT2(i_position.x, i_position.y));
  }

  void Renderer2d::renderTexture(ResourceId i_textureResourceId, const Sdk::Vector2& i_position)
  {
    const auto& resourceController = dynamic_cast<const ResourceController&>(d_resourceController);
    const auto& textureResource = resourceController.getTextureResource(i_textureResourceId);

    d_spriteBatch->Draw(textureResource.getTexturePtr(),
      XMFLOAT2(std::roundf(i_position.x), std::roundf(i_position.y)), Colors::White);
  }

  void Renderer2d::renderTexture(ResourceId i_textureResourceId,
    const Sdk::Vector2& i_position, const Sdk::Vector2& i_size)
  {
    const auto& resourceController = dynamic_cast<const ResourceController&>(d_resourceController);
    const auto& textureResource = resourceController.getTextureResource(i_textureResourceId);

    RECT destinationRect{ (int)i_position.x, (int)i_position.y,
      (int)(i_position.x + i_size.x), (int)(i_position.y + i_size.y) };
    d_spriteBatch->Draw(textureResource.getTexturePtr(), destinationRect, Colors::White);
  }

  void Renderer2d::renderSprite(const Sprite& i_sprite)
  {
    renderTexture(i_sprite.resourceId, i_sprite.position, i_sprite.size);
  }

} // ns Dx
