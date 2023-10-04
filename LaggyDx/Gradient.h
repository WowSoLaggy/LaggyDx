#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class Gradient
  {
  public:
    Gradient() = default;
    Gradient(Color i_color1, Color i_color2);

    void setColor1(Color i_color);
    void setColor2(Color i_color);

    Color get(float i_ratio) const;

  private:
    Color d_color1;
    Color d_color2;
  };

} // ns Dx
