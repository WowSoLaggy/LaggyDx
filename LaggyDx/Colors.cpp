#include "stdafx.h"
#include "Colors.h"

namespace Dx
{
  Sdk::Vector4F colorWithAlpha(Sdk::Vector4F i_color, const float i_alpha)
  {
    i_color.w = i_alpha;
    return i_color;
  }

} // ns Dx
