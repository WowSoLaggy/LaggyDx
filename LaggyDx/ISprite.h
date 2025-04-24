#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISprite
  {
  public:
    virtual ~ISprite() = default;

    virtual const ITexture* getTexture() const = 0;
    virtual const Sdk::Vector2F& getPosition() const = 0;
    virtual double getRotation() const = 0;
    virtual double getScale() const = 0;
    virtual const Sdk::Vector2F& getSize() const = 0;
    virtual const Sdk::Vector4F& getColor() const = 0;
    virtual const Sdk::Vector2F& getRotationOrigin() const = 0;
    virtual RECT getSourceRect() const = 0;

    virtual bool hasAnimation() const = 0;
    virtual int getCurrentFrame() const = 0;
    virtual float getFrameRatio() const = 0;
  };

} // ns Dx
