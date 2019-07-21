#pragma once

#include "IRenderer2d.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class Renderer2d : public IRenderer2d
  {
  public:
    Renderer2d(IRenderDevice& io_renderDevice);

    virtual void beginScene() override;
    virtual void endScene() override;


    virtual void renderText(const std::string& i_text,
      const IFontResource& i_fontResource, const Sdk::Vector2& i_position) override;

    virtual void renderTexture(const ITextureResource& i_textureResource, const Sdk::Vector2& i_position) override;
    virtual void renderTexture(const ITextureResource& i_textureResource,
      const Sdk::Vector2& i_position, const Sdk::Vector2& i_size) override;

    virtual void renderSprite(const Sprite& i_sprite) override;

  private:
    IRenderDevice& d_renderDevice;

    std::shared_ptr<SpriteBatch> d_spriteBatch;
    std::unique_ptr<CommonStates> d_states;

  };

} // ns Dx
