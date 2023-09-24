#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class GridItem
  {
  public:
    virtual ~GridItem() = default;

    void setTexture(const Dx::ITexture* i_texture);
    const Dx::ITexture* getTexture() const;

  private:
    const Dx::ITexture* d_texture;
  };

} // ns Dx
