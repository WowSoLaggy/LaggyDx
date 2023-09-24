#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  class CursorUtils
  {
  public:
    static const Sdk::Vector2I& getPosition();

  private:
    CursorUtils() = delete;
  };

} // ns Dx
