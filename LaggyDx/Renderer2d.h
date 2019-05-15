#pragma once

#include "IRenderer2d.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class Renderer2d : public IRenderer2d
  {
  public:

    Renderer2d(
      IRenderDevice& io_renderDevice,
      const IResourceController& i_resourceController);

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void renderText(const std::string& i_text, ResourceId i_fontResourceId, const Sdk::Vector2& i_position) override;
    virtual void renderTexture(ResourceId i_textureResourceId, const Sdk::Vector2& i_position) override;
    virtual void renderTexture(ResourceId i_textureResourceId,
      const Sdk::Vector2& i_position, const Sdk::Vector2& i_size) override;
    virtual void renderSprite(const Sprite& i_sprite) override;

  private:

    IRenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;

    std::shared_ptr<SpriteBatch> d_spriteBatch;

  };

} // ns Dx
