#include "stdafx.h"
#include "ObjectBase.h"

#include "Game.h"
#include "ImageDescription.h"
#include "ITextureResource.h"
#include "Renderer2dGuard.h"


namespace Dx
{
  void ObjectBase::setTexture(const std::string& i_textureName)
  {
    auto& rc = Game::get().getResourceController();
    const auto& texture = rc.getTextureResource(i_textureName);
    d_sprite.setTexture(texture);
    d_sprite.setSize(texture.getDescription().size());

    updateRotationOrigin();
  }


  void ObjectBase::setPosition(Sdk::Vector2D i_position)
  {
    d_position = std::move(i_position);

    updateTranslation();
  }

  const Sdk::Vector2D& ObjectBase::getPosition() const
  {
    return d_position;
  }


  void ObjectBase::setSpeed(Sdk::Vector2D i_speed)
  {
    d_speed = std::move(i_speed);
  }

  const Sdk::Vector2D& ObjectBase::getSpeed() const
  {
    return d_speed;
  }


  void ObjectBase::setRotation(const double i_rotation)
  {
    d_sprite.setRotation(i_rotation);
  }
  double ObjectBase::getRotation() const
  {
    return d_sprite.getRotation();
  }


  void ObjectBase::setColor(Sdk::Vector4F i_color)
  {
    d_sprite.setColor(std::move(i_color));
  }


  void ObjectBase::render(IRenderer2d& i_renderer) const
  {
    Renderer2dGuard rendererGuard(i_renderer, d_translation, d_rotationOrigin, d_sprite.getRotation());
    i_renderer.renderSprite(d_sprite);
  }

  void ObjectBase::update(const double i_dt)
  {
    setPosition(d_position + d_speed * i_dt);
  }


  void ObjectBase::updateTranslation()
  {
    d_translation = {
      d_position.x - (double)d_sprite.getSize().x / 2,
      d_position.y - (double)d_sprite.getSize().y / 2 };
  }

  void ObjectBase::updateRotationOrigin()
  {
    d_rotationOrigin = {
      (double)d_sprite.getSize().x / 2,
      (double)d_sprite.getSize().y / 2 };
  }

} // Dx
