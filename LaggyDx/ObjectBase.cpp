#include "stdafx.h"
#include "ObjectBase.h"

#include "CollShapeMover.h"
#include "Game.h"
#include "ImageDescription.h"
#include "ITextureResource.h"
#include "Renderer2dGuard.h"
#include "StaticCollider.h"


namespace Dx
{
  ObjectBase::ObjectBase()
    : d_scale(Sdk::Vector2D::identity())
    , d_collider(std::make_shared<StaticCollider>(*this))
  {
    updateRotationOrigin();
    updateTranslation();
  }


  void ObjectBase::setTexture(const std::string& i_textureName)
  {
    auto& rc = Game::get().getResourceController();
    const auto& texture = rc.getTextureResource(i_textureName);
    d_sprite.setTexture(texture);
    d_sprite.setSize(texture.getDescription().size());

    updateRotationOrigin();
    updateTranslation();
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

  Sdk::Vector2F ObjectBase::getPositionF() const
  {
    return { (float)d_position.x, (float)d_position.y };
  }


  void ObjectBase::setSpeed(Sdk::Vector2D i_speed)
  {
    d_speed = std::move(i_speed);
  }

  const Sdk::Vector2D& ObjectBase::getSpeed() const
  {
    return d_speed;
  }

  Sdk::Vector2F ObjectBase::getSpeedF() const
  {
    return { (float)d_speed.x, (float)d_speed.y };
  }


  void ObjectBase::setAccel(Sdk::Vector2D i_speed)
  {
    d_accel = std::move(i_speed);
  }

  const Sdk::Vector2D& ObjectBase::getAccel() const
  {
    return d_accel;
  }

  Sdk::Vector2F ObjectBase::getAccelF() const
  {
    return { (float)d_accel.x, (float)d_accel.y };
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
    d_color = i_color;
    d_sprite.setColor(std::move(i_color));
  }

  const Sdk::Vector4F& ObjectBase::getColor() const
  {
    return d_color;
  }



  void ObjectBase::setScale(Sdk::Vector2D i_scale)
  {
    d_scale = std::move(i_scale);
  }

  const Sdk::Vector2D& ObjectBase::getScale() const
  {
    return d_scale;
  }


  const Sprite& ObjectBase::getSprite() const
  {
    return d_sprite;
  }


  void ObjectBase::onCollide(const CollisionInfo& i_collInfo)
  {
    // nop
  }

  CollisionShape ObjectBase::getPositionedCollisionShape() const
  {
    const auto collider = getCollider();
    CONTRACT_EXPECT(collider);
    const auto& collShape = collider->getCollisionShape();
    const auto positionedCollisionShape = std::visit(CollShapeMover(getPosition()), collShape);
    return positionedCollisionShape;
  }

  std::shared_ptr<ICollider> ObjectBase::getCollider() const
  {
    return d_collider;
  }

  void ObjectBase::setCollider(std::shared_ptr<ICollider> i_collider)
  {
    d_collider = i_collider;
  }


  void ObjectBase::render(IRenderer2d& i_renderer) const
  {
    Renderer2dGuard rendererGuard(i_renderer,
                                  { (float)d_translation.x, (float)d_translation.y },
                                  { (float)d_origin.x, (float)d_origin.y },
                                  (float)d_sprite.getRotation(),
                                  { (float)d_origin.x, (float)d_origin.y },
                                  0.0f,
                                  { (float)d_scale.x, (float)d_scale.y });

    renderWithGuard(i_renderer);
  }

  void ObjectBase::renderWithGuard(IRenderer2d& i_renderer) const
  {
    i_renderer.renderSprite(d_sprite);
  }


  void ObjectBase::update(const double i_dt)
  {
  }


  void ObjectBase::updateTranslation()
  {
    d_translation = {
      d_position.x - (double)d_sprite.getSize().x / 2,
      d_position.y - (double)d_sprite.getSize().y / 2 };
  }

  void ObjectBase::updateRotationOrigin()
  {
    d_origin = {
      (double)d_sprite.getSize().x / 2,
      (double)d_sprite.getSize().y / 2 };
  }

} // Dx
