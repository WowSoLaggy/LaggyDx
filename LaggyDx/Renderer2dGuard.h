#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Renderer2dGuard
  {
  public:
    Renderer2dGuard(IRenderer2d& i_renderer);
    Renderer2dGuard(IRenderer2d& i_renderer,
                    const Sdk::Vector2F& i_translation);
    Renderer2dGuard(IRenderer2d& i_renderer,
                    const Sdk::Vector2F& i_translation,
                    const Sdk::Vector2F& i_rotationOrigin,
                    float i_rotation);
    Renderer2dGuard(IRenderer2d& i_renderer,
                    const Sdk::Vector2F& i_translation,
                    const Sdk::Vector2F& i_scalingOrigin,
                    const Sdk::Vector2F& i_scaling);
    Renderer2dGuard(IRenderer2d& i_renderer,
                    const Sdk::Vector2F& i_translation,
                    const Sdk::Vector2F& i_rotationOrigin,
                    float i_rotation,
                    const Sdk::Vector2F& i_scalingOrigin,
                    float i_scalingOrientation,
                    const Sdk::Vector2F& i_scaling);

    ~Renderer2dGuard();

  private:
    IRenderer2d& d_renderer;
  };

} // ns Dx
