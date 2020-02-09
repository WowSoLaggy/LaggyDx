#pragma once

#include <Sdk/Vector.h>


namespace Dx
{
  struct ImageDescription
  {
    int width = 0;
    int height = 0;
    bool alpha = false;

    Sdk::Vector2I size() const { return { width, height }; }
  };
} // ns Dx
