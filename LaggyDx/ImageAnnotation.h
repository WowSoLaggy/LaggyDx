#pragma once

#include "ImageAnimation.h"
#include "ImageDescription.h"

#include <unordered_map>


namespace Dx
{
  struct ImageAnnotation
  {
    ImageDescription description;
    std::unordered_map<std::string, ImageAnimation> animations;
  };
} // ns Dx
