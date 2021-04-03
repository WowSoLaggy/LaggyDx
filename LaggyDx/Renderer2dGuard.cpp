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
                                   const Sdk::Vector2F& i_translation)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene(i_translation);
  }

  Renderer2dGuard::Renderer2dGuard(IRenderer2d& i_renderer,
                                   const Sdk::Vector2F& i_translation,
                                   const Sdk::Vector2F& i_rotationOrigin,
                                   const float i_rotation)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene(i_translation, i_rotationOrigin, i_rotation);
  }

  Renderer2dGuard::Renderer2dGuard(IRenderer2d& i_renderer,
                                   const Sdk::Vector2F& i_translation,
                                   const Sdk::Vector2F& i_scaleOrigin,
                                   const Sdk::Vector2F& i_scaling)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene(i_translation, i_scaleOrigin, i_scaling);
  }

  Renderer2dGuard::Renderer2dGuard(IRenderer2d& i_renderer,
                                   const Sdk::Vector2F& i_translation,
                                   const Sdk::Vector2F& i_rotationOrigin,
                                   float i_rotation,
                                   const Sdk::Vector2F& i_scalingOrigin,
                                   float i_scalingOrientation,
                                   const Sdk::Vector2F& i_scaling)
    : d_renderer(i_renderer)
  {
    d_renderer.beginScene(i_translation, i_rotationOrigin, i_rotation,
                          i_scalingOrigin, i_scalingOrientation, i_scaling);
  }


  Renderer2dGuard::~Renderer2dGuard()
  {
    d_renderer.endScene();
  }

} // ns Dx
