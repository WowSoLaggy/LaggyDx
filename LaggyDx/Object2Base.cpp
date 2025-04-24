#include "stdafx.h"
#include "Object2Base.h"

#include "App.h"
#include "CollShapeMover.h"
#include "ImageDescription.h"
#include "ITexture.h"
#include "Renderer2dGuard.h"
#include "StaticCollider.h"


namespace Dx
{
  Object2Base::Object2Base()
    : d_scale(Sdk::Vector2D::identity())
    , d_collider(std::make_shared<StaticCollider>(*this))
  {
    updateRotationOrigin();
    updateTranslation();
  }


  void Object2Base::setTexture(const std::string& i_textureName)
  {
    const auto& rc = App::get().getResourceController();
    const auto& texture = rc.getTexture(i_textureName);
    d_sprite.setTexture(texture);
    d_sprite.resetSizeToTexture();

    updateRotationOrigin();
    updateTranslation();
  }


  void Object2Base::setPosition(Sdk::Vector2D i_position)
  {
    d_position = std::move(i_position);

    updateTranslation();
  }

  const Sdk::Vector2D& Object2Base::getPosition() const
  {
    return d_position;
  }

  Sdk::Vector2F Object2Base::getPositionF() const
  {
    return { (float)d_position.x, (float)d_position.y };
  }


  void Object2Base::setSpeed(Sdk::Vector2D i_speed)
  {
    d_speed = std::move(i_speed);
  }

  const Sdk::Vector2D& Object2Base::getSpeed() const
  {
    return d_speed;
  }

  Sdk::Vector2F Object2Base::getSpeedF() const
  {
    return { (float)d_speed.x, (float)d_speed.y };
  }


  void Object2Base::setAccel(Sdk::Vector2D i_speed)
  {
    d_accel = std::move(i_speed);
  }

  const Sdk::Vector2D& Object2Base::getAccel() const
  {
    return d_accel;
  }

  Sdk::Vector2F Object2Base::getAccelF() const
  {
    return { (float)d_accel.x, (float)d_accel.y };
  }


  void Object2Base::setRotation(const double i_rotation)
  {
    d_sprite.setRotation(i_rotation);
  }

  double Object2Base::getRotation() const
  {
    return d_sprite.getRotation();
  }


  void Object2Base::setColor(Sdk::Vector4F i_color)
  {
    d_color = i_color;
    d_sprite.setColor(std::move(i_color));
  }

  const Sdk::Vector4F& Object2Base::getColor() const
  {
    return d_color;
  }



  void Object2Base::setScale(Sdk::Vector2D i_scale)
  {
    d_scale = std::move(i_scale);
  }

  const Sdk::Vector2D& Object2Base::getScale() const
  {
    return d_scale;
  }


  const Sprite& Object2Base::getSprite() const
  {
    return d_sprite;
  }


  void Object2Base::onCollide(const CollisionInfo& i_collInfo)
  {
    // nop
  }

  CollisionShape Object2Base::getPositionedCollisionShape() const
  {
    const auto collider = getCollider();
    CONTRACT_EXPECT(collider);
    const auto& collShape = collider->getCollisionShape();
    const auto positionedCollisionShape = std::visit(CollShapeMover(getPosition()), collShape);
    return positionedCollisionShape;
  }

  std::shared_ptr<ICollider> Object2Base::getCollider() const
  {
    return d_collider;
  }

  void Object2Base::setCollider(std::shared_ptr<ICollider> i_collider)
  {
    d_collider = i_collider;
  }


  void Object2Base::render(IRenderer2d& i_renderer) const
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

  void Object2Base::renderWithGuard(IRenderer2d& i_renderer) const
  {
    i_renderer.renderSprite(d_sprite);
  }


  void Object2Base::update(const double i_dt)
  {
  }


  void Object2Base::updateTranslation()
  {
    d_translation = {
      d_position.x - (double)d_sprite.getSize().x / 2,
      d_position.y - (double)d_sprite.getSize().y / 2 };
  }

  void Object2Base::updateRotationOrigin()
  {
    d_origin = {
      (double)d_sprite.getSize().x / 2,
      (double)d_sprite.getSize().y / 2 };
  }

} // Dx
