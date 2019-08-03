#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Sprite
  {
  public:
    Sprite(const ITextureResource* i_texture = nullptr, Sdk::Vector2I i_position = Sdk::Vector2I::zero(),
           Sdk::Vector2I i_size = Sdk::Vector2I::zero(), Sdk::Vector4F i_color = Sdk::Vector4F::identity());

    void setTexture(const ITextureResource* i_texture);
    const ITextureResource* getTexture() const { return d_texture; }

    void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }
    const Sdk::Vector2I& getPosition() const { return d_position; }

    void setColor(Sdk::Vector4F i_color) { d_color = std::move(i_color); }
    const Sdk::Vector4F& getColor() const { return d_color; }

    void setSize(Sdk::Vector2I i_size) { d_size = std::move(i_size); }
    const Sdk::Vector2I& getSize() const { return d_size; }

    virtual RECT getSourceRect() const;

  protected:
    const ITextureResource* d_texture = nullptr;
    Sdk::Vector2I d_position = Sdk::Vector2I::zero();
    Sdk::Vector2I d_size = Sdk::Vector2I::zero();
    Sdk::Vector4F d_color = Sdk::Vector4F::identity();
  };

} // ns Dx
