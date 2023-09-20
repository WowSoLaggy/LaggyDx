#include "stdafx.h"
#include "GridItem.h"


namespace Dx
{
  void GridItem::setTextureName(std::string i_textureName)
  {
    d_textureName = std::move(i_textureName);
  }

  const std::string& GridItem::getTextureName() const
  {
    return d_textureName;
  }


  void GridItem::setOnClickHandler(std::function<void()> i_onClickHandler)
  {
    d_onClickHandler = i_onClickHandler;
  }


  void GridItem::onClick()
  {
    if (d_onClickHandler)
      d_onClickHandler();
  }

} // ns Dx
