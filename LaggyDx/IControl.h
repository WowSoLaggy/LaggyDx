#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/TreeNode.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class IControl : public Sdk::TreeNode
  {
  public:
    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const = 0;
    virtual void update(double i_dt) = 0;

    virtual void setPosition(Sdk::Vector2F i_position) = 0;
    virtual const Sdk::Vector2F& getPosition() const = 0;

    virtual void setSize(Sdk::Vector2F i_size) = 0;
    virtual const Sdk::Vector2F& getSize() const = 0;

    virtual void setOpacity(double i_opacity) = 0;
    virtual double getOpacity() const = 0;
  };

} // Dx
