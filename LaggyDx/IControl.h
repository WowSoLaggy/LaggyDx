#pragma once

#include "LaggyDxFwd.h"
#include "MouseKeys.h"

#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/TreeNode.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class IControl : public Sdk::TreeNode, public Sdk::EventHandler
  {
  public:
    virtual void render(IRenderer2d& i_renderer) const = 0;
    virtual void update(double i_dt) = 0;

    virtual void setPosition(Sdk::Vector2F i_position) = 0;
    virtual Sdk::Vector2F getPositionAbsolute() const = 0;
    virtual Sdk::Vector2F getPositionRelative() const = 0;

    virtual void setSize(Sdk::Vector2F i_size) = 0;
    virtual Sdk::Vector2F getSize() const = 0;

    virtual Sdk::RectF getRectAbsolute() const = 0;

    virtual void setOpacity(double i_opacity) = 0;
    virtual double getOpacity() const = 0;

    virtual void setVisible(bool i_visible) = 0;
    virtual bool getVisible() const = 0;

    virtual void onMouseMove() = 0;
    virtual void onMouseClick(MouseKey i_key) = 0;
    virtual void onMouseRelease(MouseKey i_key) = 0;

    virtual void addEffect(std::shared_ptr<IGuiEffect> i_effect) = 0;
  };

} // Dx
