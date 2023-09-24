#include "stdafx.h"
#include "GridItem.h"


namespace Dx
{
  void GridItem::setTexture(const Dx::ITexture* i_texture)
  {
    d_texture = i_texture;
  }

  const Dx::ITexture* GridItem::getTexture() const
  {
    return d_texture;
  }

} // ns Dx
