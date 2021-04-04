#pragma once

#include "IRenderer2d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Renderer2d : public IRenderer2d
  {
  public:
    Renderer2d(IRenderDevice& io_renderDevice, Sdk::Vector2I i_resolution);

    virtual void beginScene() override;
    virtual void beginScene(const Sdk::Vector2F& i_translation) override;
    virtual void beginScene(const Sdk::Vector2F& i_translation,
                            const Sdk::Vector2F& i_rotationOrigin,
                            float i_rotation) override;
    virtual void beginScene(const Sdk::Vector2F& i_translation,
                            const Sdk::Vector2F& i_scaleOrigin,
                            const Sdk::Vector2F& i_scaling) override;
    virtual void beginScene(const Sdk::Vector2F& i_translation,
                            const Sdk::Vector2F& i_rotationOrigin,
                            float i_rotation,
                            const Sdk::Vector2F& i_scalingOrigin,
                            float i_scalingOrientation,
                            const Sdk::Vector2F& i_scaling) override;
    virtual void endScene() override;

    virtual const Sdk::Vector2F& getTranslation() const override;
    virtual void setTranslation(Sdk::Vector2F i_translation) override;
    virtual void resetTranslation() override;

    virtual int resetRenderedSprites() override;


    virtual void renderText(const std::string& i_text,
                            const IFontResource& i_fontResource,
                            const Sdk::Vector2F& i_position) override;
    virtual void renderSprite(const Sprite& i_sprite) override;
    virtual void renderLine(const Sdk::Vector2F& i_start, const Sdk::Vector2F& i_end,
                            const Sdk::Vector4F& i_color) override;
    virtual void renderRect(const Sdk::RectI& i_rect, const Sdk::Vector4F& i_color) override;
    virtual void renderShape(const IShape& i_shape) override;

  private:
    RenderDevice& d_renderDevice;
    Sdk::Vector2I d_resolution;

    std::unique_ptr<CommonStates> d_states;

    SpriteBatch d_spriteBatch;
    PrimitiveBatch<VertexPositionColor> d_primitiveBatch;
    BasicEffect d_primitiveEffect;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> d_primitiveInputLayout;

    Sdk::Vector2F d_translation;

    int d_renderedSprites = 0;
  };

} // ns Dx
