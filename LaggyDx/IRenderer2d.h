#pragma once

#include "IRenderer.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/SdkFwd.h>


namespace Dx
{
  class IRenderer2d : public IRenderer
  {
  public:

    static std::unique_ptr<IRenderer2d> create(
      IRenderDevice& io_renderDevice,
      const IResourceController& i_resourceController);

  public:

    virtual ~IRenderer2d() = default;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void renderText(const std::string& i_text, ResourceId i_fontResourceId, const Sdk::Vector2& i_position) = 0;
    virtual void renderTexture(ResourceId i_textureResourceId, const Sdk::Vector2& i_position) = 0;
    virtual void renderTexture(ResourceId i_textureResourceId,
      const Sdk::Vector2& i_position, const Sdk::Vector2& i_size) = 0;
    virtual void renderSprite(const Sprite& i_sprite) = 0;
  };

} // ns Dx
