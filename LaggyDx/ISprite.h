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
    virtual const Sdk::Vector2I& getPosition() const = 0;
    virtual double getRotation() const = 0;
    virtual double getScale() const = 0;
    virtual const Sdk::Vector2I& getSize() const = 0;
    virtual const Sdk::Vector4F& getColor() const = 0;
    virtual RECT getSourceRect() const = 0;
  };

} // ns Dx
