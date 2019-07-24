#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Sprite
  {
  public:
    void setTexture(const ITextureResource* i_texture) { d_texture = i_texture; }
    const ITextureResource* getTexture() const { return d_texture; }

    void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }
    const Sdk::Vector2I& getPosition() const { return d_position; }

  private:
    const ITextureResource* d_texture = nullptr;
    Sdk::Vector2I d_position = Sdk::Vector2I::zero();
  };

} // ns Dx
