#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


struct Material
{
  std::string name;

  Sdk::Vector4 ambientColor;
  Sdk::Vector4 diffuseColor;

  static Material getDefault() { return { "", { 1, 1, 1, 1}, {1, 1, 1, 1} }; }
};
