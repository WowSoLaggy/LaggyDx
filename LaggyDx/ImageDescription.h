#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct ImageDescription
  {
    Sdk::Vector2I size;
    Sdk::Vector2I frameSize;

    bool alpha = false;
  };
} // ns Dx
