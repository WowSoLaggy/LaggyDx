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
    virtual void setPosition(Sdk::Vector3F i_position) = 0;
    virtual void setRotation(Sdk::Vector3F i_rotation) = 0;
    virtual void setScale(Sdk::Vector3F i_scale) = 0;

    virtual const IModel& getModel() const = 0;
    virtual const ITextureResource* getTextureResource() const = 0;

    virtual void setModel(std::shared_ptr<IModel> i_model) = 0;
    virtual void setTextureResource(const ITextureResource& i_textureResource) = 0;
  };

} // ns Dx
