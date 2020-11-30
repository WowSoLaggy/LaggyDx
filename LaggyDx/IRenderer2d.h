#pragma once

#include "IRenderer.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class IRenderer2d : public IRenderer
  {
  public:
    static std::unique_ptr<IRenderer2d> create(IRenderDevice& io_renderDevice, Sdk::Vector2I i_resolution);

  public:
    virtual ~IRenderer2d() = default;

    virtual void beginScene() = 0;
    virtual void beginScene(const Sdk::Vector2I& i_translation,
                            const Sdk::Vector2I& i_scaleOrigin,
                            const Sdk::Vector2D& i_scaling) = 0;
    virtual void endScene() = 0;

    virtual const Sdk::Vector2I& getTranslation() const = 0;
    virtual void setTranslation(Sdk::Vector2I i_translation) = 0;
    virtual void resetTranslation() = 0;

    virtual int getRenderedSprites() = 0;


    virtual void renderText(const std::string& i_text,
      const IFontResource& i_fontResource, const Sdk::Vector2I& i_position) = 0;
    virtual void renderSprite(const Sprite& i_sprite) = 0;
    virtual void renderLine(const Sdk::Vector2I& i_start, const Sdk::Vector2I& i_end,
                            const Sdk::Vector4F& i_color) = 0;
    virtual void renderRect(const Sdk::RectI& i_rect, const Sdk::Vector4F& i_color) = 0;
  };

} // ns Dx
