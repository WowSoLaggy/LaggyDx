#include "stdafx.h"
#include "Gradient.h"


namespace Dx
{
  Gradient::Gradient(Color i_color1, Color i_color2)
    : d_color1(std::move(i_color1))
    , d_color2(std::move(i_color2))
  {
  }


  void Gradient::setColor1(Color i_color)
  {
    d_color1 = std::move(i_color);
  }

  void Gradient::setColor2(Color i_color)
  {
    d_color2 = std::move(i_color);
  }


  Color Gradient::get(const float i_ratio) const
  {
    return d_color1 + (d_color2 - d_color1) * i_ratio;
  }

} // ns Dx
