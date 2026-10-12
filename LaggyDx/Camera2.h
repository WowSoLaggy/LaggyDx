#pragma once

#include "ICamera2.h"


namespace Dx
{
  class Camera2 : public ICamera2
  {
  public:
    virtual const Sdk::Vector2F& getOffset() const override;
    virtual void setOffset(Sdk::Vector2F i_offset) override;
    virtual void move(const Sdk::Vector2F& i_offset) override;

    virtual float getZoom() const override;
    virtual void setZoom(float i_zoom) override;

  private:
    Sdk::Vector2F d_offset;
    float d_zoom = 1;
  };

} // ns Dx
