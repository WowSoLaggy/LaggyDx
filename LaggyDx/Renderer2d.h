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

    virtual const Sdk::Vector2I& getTranslation() const override;
    virtual void setTranslation(Sdk::Vector2I i_translation) override;
    virtual void resetTranslation() override;

    virtual int getRenderedSprites() override;


    virtual void renderText(const std::string& i_text,
      const IFontResource& i_fontResource, const Sdk::Vector2I& i_position) override;
    
    virtual void renderSprite(const Sprite& i_sprite) override;

  private:
    IRenderDevice& d_renderDevice;

    std::shared_ptr<SpriteBatch> d_spriteBatch;
    std::unique_ptr<CommonStates> d_states;

    Sdk::Vector2I d_translation{};

    int d_renderedSprites = 0;
  };

} // ns Dx
