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
                    const Sdk::Vector2D& i_translation,
                    const Sdk::Vector2D& i_rotationOrigin,
                    double i_rotation);
    Renderer2dGuard(IRenderer2d& i_renderer,
                    const Sdk::Vector2I& i_translation,
                    const Sdk::Vector2I& i_scaleOrigin,
                    const Sdk::Vector2D& i_scaling);
    ~Renderer2dGuard();

  private:
    IRenderer2d& d_renderer;
  };

} // ns Dx
