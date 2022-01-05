#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Material
  {
    static Material getDefault() { return { "", { 1, 1, 1, 1}, {1, 1, 1, 1} }; }
    
    std::string name;

    Sdk::Vector4F ambientColor;
    Sdk::Vector4F diffuseColor;
  };

} // ns Dx
