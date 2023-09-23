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

    if (d_selectedIndex)
      i_renderer.renderSprite(d_selectionSprite);
  }


  void Grid::onMouseClick(MouseKey i_key)
  {
    if (i_key == MouseKey::Right)
      onRightMouseClick();
    if (i_key == MouseKey::Left)
      onLeftMouseClick();

    Control::onMouseClick(i_key);
  }

  void Grid::onLeftMouseClick()
  {
    if (!d_selectionEnabled)
      return;

    const auto& cursorPos = App::get().getInputDevice().getMousePosition() - getPositionAbsolute().getVector<int>();
    for (int ind = 0; ind < d_slotSprites.size(); ++ind)
    {
      if (d_slotSprites[ind].getRect().containsPoint(cursorPos))
      {
        selectItem(ind);
        return;
      }
    }
  }

  void Grid::onRightMouseClick()
  {
    unselectItem();
  }


  void Grid::resize(const int i_slotsX, const int i_slotsY)
  {
    d_slotsX = i_slotsX;
    d_slotsY = i_slotsY;
    recreateGridSprites();
  }


  void Grid::setTextures(
    std::string i_textureName_T,
    std::string i_textureName_TL,
    std::string i_textureName_TR,
    std::string i_textureName_L,
    std::string i_textureName_R,
    std::string i_textureName_B,
    std::string i_textureName_BL,
    std::string i_textureName_BR,
    std::string i_textureName_Slot,
    std::string i_textureName_Selection)
  {
    d_textureName_T = i_textureName_T;
    d_textureName_TL = i_textureName_TL;
    d_textureName_TR = i_textureName_TR;
    d_textureName_L = i_textureName_L;
    d_textureName_R = i_textureName_R;
    d_textureName_B = i_textureName_B;
    d_textureName_BL = i_textureName_BL;
    d_textureName_BR = i_textureName_BR;
    d_textureName_Slot = i_textureName_Slot;
    d_textureName_Selection = i_textureName_Selection;

    recreateGridSprites();
  }


  void Grid::recreateGridSprites()
  {
    if (
      d_textureName_T.empty() ||
      d_textureName_TL.empty() ||
      d_textureName_TR.empty() ||
      d_textureName_L.empty() ||
      d_textureName_R.empty() ||
      d_textureName_B.empty() ||
      d_textureName_BL.empty() ||
      d_textureName_BR.empty() ||
      d_textureName_Slot.empty())
    {
      return;
    }

    const auto& texture_T(getTexture(d_textureName_T));
    const auto& texture_TL(getTexture(d_textureName_TL));
    const auto& texture_TR(getTexture(d_textureName_TR));
    const auto& texture_L(getTexture(d_textureName_L));
    const auto& texture_R(getTexture(d_textureName_R));
    const auto& texture_B(getTexture(d_textureName_B));
    const auto& texture_BL(getTexture(d_textureName_BL));
    const auto& texture_BR(getTexture(d_textureName_BR));
    const auto& texture_Slot(getTexture(d_textureName_Slot));

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

    recreateItemSprites();
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


  void Grid::setSelectionEnabled(const bool i_selectionEnabled)
  {
    i_selectionEnabled ? onSelectionEnabled() : onSelectionDisabled();
  }

  void Grid::setEmptySelectionEnabled(const bool i_emptySelectionEnabled)
  {
    i_emptySelectionEnabled ? onEmptySelectionEnabled() : onEmptySelectionDisabled();
  }

  void Grid::onSelectionEnabled()
  {
    d_selectionEnabled = true;

    d_selectionSprite.setTexture(getTexture(d_textureName_Selection));
    d_selectionSprite.resetSizeToTexture();
  }

  void Grid::onSelectionDisabled()
  {
    d_selectionEnabled = false;
    unselectItem();
  }

  void Grid::onEmptySelectionEnabled()
  {
    d_emptySelectionEnabled = true;
  }

  void Grid::onEmptySelectionDisabled()
  {
    d_emptySelectionEnabled = false;

    if (d_selectedIndex && hasItemAt(*d_selectedIndex))
      unselectItem();
  }

  bool Grid::hasItemAt(const int i_index)
  {
    if (i_index < 0 || i_index >= d_items.size())
      return false;
    return d_items[i_index] != nullptr;
  }

  void Grid::selectItem(const int i_itemIndex)
  {
    if (!d_selectionEnabled)
      return;
    if (!d_emptySelectionEnabled && !hasItemAt(i_itemIndex))
      return;

    d_selectedIndex = i_itemIndex;

    const Sdk::Vector2I itemPos(i_itemIndex % d_slotsX, i_itemIndex / d_slotsX);
    const Sdk::Vector2I cornerSize(d_cornerWidth, d_cornerWidth);
    const Sdk::Vector2I slotSize(d_slotWidth, d_slotHeight);
    d_selectionSprite.setPosition(cornerSize + itemPos * slotSize);

    if (d_onItemSelectedHandler)
      d_onItemSelectedHandler(SAFE_DEREF(d_items.at(*d_selectedIndex)));
  }

  void Grid::unselectItem()
  {
    d_selectedIndex = std::nullopt;

    if (d_onItemUnselectedHandler)
      d_onItemUnselectedHandler();
  }


  void Grid::setOnItemSelected(OnGridItemSelectedHandler i_handler)
  {
    d_onItemSelectedHandler = std::move(i_handler);
  }

  void Grid::setOnItemUnselected(OnGridItemUnselectedHandler i_handler)
  {
    d_onItemUnselectedHandler = std::move(i_handler);
  }

} // ns Dx
