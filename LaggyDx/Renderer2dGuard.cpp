#include "stdafx.h"
#include "Renderer2dGuard.h"

#include "IRenderer2d.h"


namespace Dx
{
  Renderer2dGuard::Renderer2dGuard(IRenderer2d& i_renderer)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene();
  }

  Renderer2dGuard::Renderer2dGuard(IRenderer2d& i_renderer,
                                   const Sdk::Vector2I& i_translation,
                                   const Sdk::Vector2I& i_scaleOrigin,
                                   const Sdk::Vector2D& i_scaling)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene(i_translation, i_scaleOrigin, i_scaling);
  }


  Renderer2dGuard::~Renderer2dGuard()
  {
    d_renderer.endScene();
  }

} // ns Dx
