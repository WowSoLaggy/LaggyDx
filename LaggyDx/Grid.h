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

    void setItems(GridItems i_items);

  private:
    int d_slotsX = 0;
    int d_slotsY = 0;

    std::vector<Dx::Sprite> d_gridSprites;
    std::vector<Dx::Sprite> d_slotSprites;

    GridItems d_items;

    void recreateGridSprites();
  };

} // ns Dx
