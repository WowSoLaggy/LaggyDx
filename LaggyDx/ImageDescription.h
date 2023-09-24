#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct ImageDescription
  {
    int width = 0;
    int height = 0;

    int frameWidth = 0;
    int frameHeight = 0;

    bool alpha = false;

    Sdk::Vector2I size() const { return { width, height }; }
    Sdk::Vector2I frameSize() const { return { frameWidth, frameHeight }; }
  };
} // ns Dx
