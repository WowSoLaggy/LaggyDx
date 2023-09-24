#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  class CursorUtils
  {
  public:
    CursorUtils() = delete;

    static const Sdk::Vector2I& getPosition();
  };

} // ns Dx
