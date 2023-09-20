#include "stdafx.h"
#include "Grid.h"

#include "App.h"
#include "ImageDescription.h"
#include "IRenderer2d.h"
#include "ITexture.h"
#include "GridItem.h"


namespace Dx
{
  namespace
  {
    const std::string TextureName_T = "Grid_T.png";
    const std::string TextureName_TL = "Grid_TL.png";
    const std::string TextureName_TR = "Grid_TR.png";
    const std::string TextureName_L = "Grid_L.png";
    const std::string TextureName_R = "Grid_R.png";
    const std::string TextureName_B = "Grid_B.png";
    const std::string TextureName_BL = "Grid_BL.png";
    const std::string TextureName_BR = "Grid_BR.png";
    const std::string TextureName_Slot = "Grid_Slot.png";
    const std::string TextureName_Selection = "Grid_Selection.png";

    const Dx::ITexture& getTexture(const fs::path& i_name)
    {
      const auto& rc = Dx::App::get().getResourceController();
      return rc.getTexture(i_name);
    }

  } // anonym NS


  Grid::Grid(const int i_slotsX, const int i_slotsY)
  {
    resize(i_slotsX, i_slotsY);
  }


  void Grid::render(Dx::IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPositionAbsolute());

    for (const auto& sprite : d_gridSprites)
      i_renderer.renderSprite(sprite);
    for (const auto& sprite : d_slotSprites)
      i_renderer.renderSprite(sprite);
    for (const auto& sprite : d_itemSprites)
      i_renderer.renderSprite(sprite);
  }


  void Grid::resize(const int i_slotsX, const int i_slotsY)
  {
    d_slotsX = i_slotsX;
    d_slotsY = i_slotsY;
    recreateGridSprites();
  }


  void Grid::recreateGridSprites()
  {
    const auto& texture_T(getTexture(TextureName_T));
    const auto& texture_TL(getTexture(TextureName_TL));
    const auto& texture_TR(getTexture(TextureName_TR));
    const auto& texture_L(getTexture(TextureName_L));
    const auto& texture_R(getTexture(TextureName_R));
    const auto& texture_B(getTexture(TextureName_B));
    const auto& texture_BL(getTexture(TextureName_BL));
    const auto& texture_BR(getTexture(TextureName_BR));
    const auto& texture_Slot(getTexture(TextureName_Slot));
    const auto& texture_Selection(getTexture(TextureName_Selection));

    d_slotWidth = texture_Slot.getDescription().size().x;
    d_slotHeight = texture_Slot.getDescription().size().y;
    d_cornerWidth = texture_TL.getDescription().size().x;
    d_cornerHeight = texture_TL.getDescription().size().y;

    d_gridSprites.push_back(Dx::Sprite{ &texture_TL, { 0, 0 } });
    d_gridSprites.push_back(Dx::Sprite{ &texture_TR, { d_cornerWidth + d_slotsX * d_slotWidth, 0 } });
    d_gridSprites.push_back(Dx::Sprite{ &texture_BL, { 0, d_cornerHeight + d_slotsY * d_slotHeight } });
    d_gridSprites.push_back(Dx::Sprite{ &texture_BR, { d_cornerWidth + d_slotsX * d_slotWidth, d_cornerHeight + d_slotsY * d_slotHeight } });

    for (int i = 0; i < d_slotsX; ++i)
    {
      d_gridSprites.push_back(Dx::Sprite{ &texture_T, { d_cornerWidth + d_slotWidth * i, 0 } });
      d_gridSprites.push_back(Dx::Sprite{ &texture_B, { d_cornerWidth + d_slotWidth * i, d_cornerHeight + d_slotsY * d_slotHeight } });
    }

    for (int i = 0; i < d_slotsY; ++i)
    {
      d_gridSprites.push_back(Dx::Sprite{ &texture_L, { 0, d_cornerHeight + d_slotHeight * i } });
      d_gridSprites.push_back(Dx::Sprite{ &texture_R, { d_cornerWidth + d_slotWidth * d_slotsX, d_cornerHeight + d_slotHeight * i } });
    }

    for (int y = 0; y < d_slotsY; ++y)
    {
      for (int x = 0; x < d_slotsX; ++x)
        d_slotSprites.push_back(Dx::Sprite{ &texture_Slot, { d_cornerWidth + d_slotWidth * x, d_cornerHeight + d_slotHeight * y } });
    }

    for (auto& sprite : d_gridSprites)
      sprite.resetSizeToTexture();
    for (auto& sprite : d_slotSprites)
      sprite.resetSizeToTexture();
  }

  void Grid::recreateItemSprites()
  {
    const Sdk::Vector2I borderSize(d_cornerWidth, d_cornerHeight);
    const Sdk::Vector2I slotSize(d_slotWidth, d_slotHeight);
    const Sdk::Vector2I slotHalfSize(d_slotWidth / 2, d_slotHeight / 2);

    int itemIndex = -1;
    for (const auto& itemPtr : d_items)
    {
      ++itemIndex;
      if (itemPtr == nullptr)
        continue;
      
      Dx::Sprite sprite(&getTexture(itemPtr->getTextureName()));
      sprite.resetSizeToTexture();

      const Sdk::Vector2I itemPos(itemIndex % d_slotsX, itemIndex / d_slotsX);
      const Sdk::Vector2I itemHalfSize(sprite.getSize() / 2);
      const auto pos = borderSize + itemPos * slotSize + (slotHalfSize - itemHalfSize) + Sdk::Vector2I{1, 1};
      sprite.setPosition(pos);

      d_itemSprites.push_back(std::move(sprite));
    }
  }


  void Grid::setItems(GridItems i_items)
  {
    d_items = std::move(i_items);
    recreateItemSprites();
  }

} // ns Dx
