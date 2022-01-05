#pragma once

#include "Material.h"


namespace Dx
{
  struct MaterialSpan
  {
    Material material;

    int startIndex = 0;
    int count = 0;
  };

} // ns Dx
