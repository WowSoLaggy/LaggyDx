#pragma once

#include "Control.h"
#include "LaggyDxFwd.h"
#include "Sprite.h"


namespace Dx
{
  using OnGridItemSelectedHandler = std::function<void(const GridItem&)>;
  using OnGridItemUnselectedHandler = std::function<void()>;


  class Grid : public Dx::Control
  {
  public:
    Grid(int i_slotsX, int i_slotsY);

    virtual void render(Dx::IRenderer2d& i_renderer) const override;
    virtual bool onMouseClick(MouseKey i_key) override;

    void resize(int i_slotsX, int i_slotsY);

    void setTextures(
      std::string i_textureName_T,
      std::string i_textureName_TL,
      std::string i_textureName_TR,
      std::string i_textureName_L,
      std::string i_textureName_R,
      std::string i_textureName_B,
      std::string i_textureName_BL,
      std::string i_textureName_BR,
      std::string i_textureName_Slot,
      std::string i_textureName_Selection);

    void setItems(GridItems i_items);

    void setSelectionEnabled(bool i_selectionEnabled);
    void setEmptySelectionEnabled(bool i_emptySelectionEnabled);
    void selectItem(int i_itemIndex);
    void unselectItem();

    void setOnItemSelected(OnGridItemSelectedHandler i_handler);
    void setOnItemUnselected(OnGridItemUnselectedHandler i_handler);

  private:
    int d_slotsX = 0;
    int d_slotsY = 0;

    std::string d_textureName_T;
    std::string d_textureName_TL;
    std::string d_textureName_TR;
    std::string d_textureName_L;
    std::string d_textureName_R;
    std::string d_textureName_B;
    std::string d_textureName_BL;
    std::string d_textureName_BR;
    std::string d_textureName_Slot;
    std::string d_textureName_Selection;

    int d_cornerWidth = 0;
    int d_cornerHeight = 0;
    int d_slotWidth = 0;
    int d_slotHeight = 0;

    std::vector<Dx::Sprite> d_gridSprites;
    std::vector<Dx::Sprite> d_slotSprites;
    std::vector<Dx::Sprite> d_itemSprites;
    Dx::Sprite d_selectionSprite;

    GridItems d_items;

    OnGridItemSelectedHandler d_onItemSelectedHandler;
    OnGridItemUnselectedHandler d_onItemUnselectedHandler;

    bool d_selectionEnabled = false;
    bool d_emptySelectionEnabled = false;
    std::optional<int> d_selectedIndex;
    void onSelectionEnabled();
    void onSelectionDisabled();
    void onEmptySelectionEnabled();
    void onEmptySelectionDisabled();
    bool hasItemAt(int i_index);

    void recreateGridSprites();
    void recreateItemSprites();

    bool onLeftMouseClick();
    void onRightMouseClick();
  };

} // ns Dx
