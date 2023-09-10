#pragma once

#include "ICamera2.h"


namespace Dx
{
  class Camera2 : public ICamera2
  {
  public:
    virtual const Sdk::Vector2I& getOffset() const override;
    virtual void setOffset(Sdk::Vector2I i_offset) override;

  private:
    Sdk::Vector2I d_offset;
  };

} // ns Dx
