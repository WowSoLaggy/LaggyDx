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
    virtual void renderLine(const Sdk::Vector2I& i_start, const Sdk::Vector2I& i_end,
                            const Sdk::Vector4F& i_color) override;
    virtual void renderRect(const Sdk::RectI& i_rect, const Sdk::Vector4F& i_color) override;

  private:
    IRenderDevice& d_renderDevice;

    std::unique_ptr<CommonStates> d_states;
    std::shared_ptr<SpriteBatch> d_textBatch;

    std::shared_ptr<PrimitiveBatch<VertexPositionColor>> d_primitiveBatch;
    std::unique_ptr<BasicEffect> d_primitiveEffect;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> d_primitiveInputLayout;

    Sdk::Vector2I d_translation{};

    int d_renderedSprites = 0;
  };

} // ns Dx
