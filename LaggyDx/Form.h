#pragma once

#include "Control.h"


namespace Dx
{
  class Form : public Control
  {
  public:
    void render(IRenderer2d& i_renderer) const;
  };

} // Dx
