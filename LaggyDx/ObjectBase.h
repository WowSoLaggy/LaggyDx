#pragma once

#include "IObject.h"
#include "Sprite.h"


namespace Dx
{
  class ObjectBase : public IObject
  {
  public:
    ObjectBase();

    virtual void setTexture(const std::string& i_textureName) override;

    virtual void setPosition(Sdk::Vector2D i_position) override;
    virtual const Sdk::Vector2D& getPosition() const override;
    virtual Sdk::Vector2F getPositionF() const override;

    virtual void setSpeed(Sdk::Vector2D i_speed) override;
    virtual const Sdk::Vector2D& getSpeed() const override;
    virtual Sdk::Vector2F getSpeedF() const override;

    virtual void setRotation(double i_rotation) override;
    virtual double getRotation() const override;

    virtual void setColor(Sdk::Vector4F i_color) override;

    virtual void setScale(Sdk::Vector2D i_scale) override;
    virtual const Sdk::Vector2D& getScale() const override;

    virtual void render(IRenderer2d& i_renderer) const override;
    virtual void update(double i_dt) override;

  private:
    Sprite d_sprite;
    Sdk::Vector2D d_position;
    Sdk::Vector2D d_speed;

    Sdk::Vector2D d_translation;
    Sdk::Vector2D d_origin;

    Sdk::Vector2D d_scale;

    void updateTranslation();
    void updateRotationOrigin();
  };

} // ns Dx
