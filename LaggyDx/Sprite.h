#pragma once

#include "ISprite.h"


namespace Dx
{
  class Sprite : public ISprite
  {
  public:
    Sprite(const ITexture* i_texture = nullptr, Sdk::Vector2F i_position = Sdk::Vector2F::zero(),
           Sdk::Vector2F i_size = Sdk::Vector2F::zero(), Sdk::Vector4F i_color = Sdk::Vector4F::identity());

    virtual const ITexture* getTexture() const override { return d_texture; }
    virtual const Sdk::Vector2F& getPosition() const override { return d_position; }
    virtual double getRotation() const override { return d_rotation; }
    virtual double getScale() const override { return d_scale; }
    virtual const Sdk::Vector2F& getSize() const override { return d_size; }
    virtual const Sdk::Vector4F& getColor() const override { return d_color; }
    virtual const Sdk::Vector2F& getRotationOrigin() const override { return d_rotationOrigin; }
    virtual RECT getSourceRect() const override;

    void setTexture(const ITexture* i_texture);
    void setTexture(const ITexture& i_texture);
    void setTexture(const std::string& i_textureName);

    void setPosition(Sdk::Vector2F i_position) { d_position = std::move(i_position); }

    Sdk::RectF getRect() const;

    void setColor(Sdk::Vector4F i_color) { d_color = std::move(i_color); }

    void setSize(Sdk::Vector2F i_size) { d_size = std::move(i_size); }
    void resetSizeToTexture();

    void setRotation(double i_rotation) { d_rotation = i_rotation; }
    void setRotationOrigin(Sdk::Vector2F i_rotationOrigin) { d_rotationOrigin = std::move(i_rotationOrigin); }

    virtual bool hasAnimation() const override;
    virtual int getCurrentFrame() const override;
    virtual float getFrameRatio() const override;

  protected:
    const ITexture* d_texture = nullptr;
    Sdk::Vector2F d_position = Sdk::Vector2F::zero();
    Sdk::Vector2F d_size = Sdk::Vector2F::zero();
    Sdk::Vector4F d_color = Sdk::Vector4F::identity();
    Sdk::Vector2F d_rotationOrigin = Sdk::Vector2F::zero();
    double d_rotation = 0.0;
    double d_scale = 1.0;

  public:
    virtual void writeTo(std::ostream& io_stream) const;
    virtual void readFrom(std::istream& io_stream, IResourceController& i_resourceController);
  };
} // ns Dx
