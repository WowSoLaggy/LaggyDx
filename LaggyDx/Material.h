#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Material
  {
    std::string name;

    Sdk::Vector4F ambientColor;
    Sdk::Vector4F diffuseColor;

    static Material getDefault() { return { "", { 1, 1, 1, 1}, {1, 1, 1, 1} }; }
  };

} // ns Dx
