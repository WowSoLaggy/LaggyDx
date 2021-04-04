#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IControl
  {
  public:
    virtual ~IControl() = default;

    virtual IControl* getParent() = 0;
    virtual const IControl* getParent() const = 0;
    virtual void setParent(IControl* i_parent) = 0;

    virtual void addChild(std::shared_ptr<IControl> i_child) = 0;
    virtual void removeChild(IControl& i_child) = 0;
    virtual std::vector<std::shared_ptr<IControl>>& getChildren() = 0;
    virtual const std::vector<std::shared_ptr<IControl>>& getChildren() const = 0;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const = 0;
    virtual void update(double i_dt) = 0;

    virtual void setPosition(Sdk::Vector2F i_position) = 0;
    virtual const Sdk::Vector2F& getPosition() const = 0;

    virtual void setSize(Sdk::Vector2F i_size) = 0;
    virtual const Sdk::Vector2F& getSize() const = 0;
  };

} // Dx
