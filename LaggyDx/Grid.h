#pragma once

#include "Control.h"
#include "LaggyDxFwd.h"
#include "Sprite.h"


namespace Dx
{
  class Grid : public Dx::Control
  {
  public:
    Grid(int i_slotsX, int i_slotsY);

    virtual void render(Dx::IRenderer2d& i_renderer) const override;

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

    GridItems d_items;

    void recreateGridSprites();
    void recreateItemSprites();
  };

} // ns Dx
