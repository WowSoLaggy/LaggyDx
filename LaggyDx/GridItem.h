#pragma once

#include "LaggyDxFwd.h"

#include <string>


namespace Dx
{
  class GridItem
  {
  public:
    virtual ~GridItem() = default;

    void setTexture(const Dx::ITexture* i_texture);
    void setTexture(const Dx::ITexture& i_texture);
    void setTexture(const std::string& i_textureName);
    const Dx::ITexture* getTexture() const;

  private:
    const Dx::ITexture* d_texture;
  };

} // ns Dx
