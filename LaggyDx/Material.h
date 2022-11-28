#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Material
  {
    std::string name;

    std::string textureName;
    Sdk::Vector4F ambientColor = Sdk::Vector4F::identity();
    Sdk::Vector4F diffuseColor = Sdk::Vector4F::identity();
    float specularPower = 16.0f;
  };

} // ns Dx
