#pragma once

#include "IControl.h"


namespace Dx
{
  class Control : public IControl, public std::enable_shared_from_this<Control>
  {
  public:
    virtual IControl* getParent() override;
    virtual const IControl* getParent() const override;
    virtual void setParent(IControl* i_parent) override;

    virtual void addChild(std::shared_ptr<IControl> i_child) override;
    virtual void removeChild(IControl& i_child) override;
    virtual std::vector<std::shared_ptr<IControl>>& getChildren() override;
    virtual const std::vector<std::shared_ptr<IControl>>& getChildren() const override;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;
    virtual void update(double i_dt) override;

    virtual void setPosition(Sdk::Vector2F i_position) override;
    virtual const Sdk::Vector2F& getPosition() const override;

    virtual void setSize(Sdk::Vector2F i_size) override;
    virtual const Sdk::Vector2F& getSize() const override;

  private:
    IControl* d_parent = nullptr;
    std::vector<std::shared_ptr<IControl>> d_children;

    Sdk::Vector2F d_position;
    Sdk::Vector2F d_size;
  };

} // Dx
