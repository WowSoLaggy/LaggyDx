#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IObject3
  {
  public:
    virtual ~IObject3() = default;

    virtual Sdk::Vector3F getPosition() const = 0;
    virtual Sdk::Vector3F getRotation() const = 0;
    virtual Sdk::Vector3F getScale() const = 0;

    virtual const IModel& getModel() const = 0;
    virtual const ITextureResource& getTextureResource() const = 0;
  };

} // ns Dx
