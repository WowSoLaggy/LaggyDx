#include "stdafx.h"
#include "Panel.h"

#include "IRenderer2d.h"


namespace Dx
{
  void Panel::setTexture(const ITexture& i_texture)
  {
    d_sprite.setTexture(i_texture);
  }

  void Panel::setColor(Sdk::Vector4F i_color)
  {
    d_sprite.setColor(std::move(i_color));
  }

  void Panel::setRotation(float i_rotation)
  {
    d_sprite.setRotation(i_rotation);
  }

  void Panel::setRotationOrigin(Sdk::Vector2F i_rotationOrigin)
  {
    d_sprite.setRotationOrigin(std::move(i_rotationOrigin));
  }


  void Panel::setSize(Sdk::Vector2F i_size)
  {
    d_sprite.setSize({ i_size.x, i_size.y });
    Control::setSize(i_size);
  }


  void Panel::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());
    i_renderer.renderSprite(d_sprite);

    Control::render(i_renderer);
  }

} // ns Dx
