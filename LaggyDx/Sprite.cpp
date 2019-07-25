#include "stdafx.h"
#include "Sprite.h"

#include "ImageDescription.h"
#include "ITextureResource.h"


namespace Dx
{
  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, d_texture->getDescription().width - 1, d_texture->getDescription().height - 1 };
  }

  const Sdk::Vector4F& Sprite::getColor() const
  {
    return d_color;
  }
} // ns Dx
