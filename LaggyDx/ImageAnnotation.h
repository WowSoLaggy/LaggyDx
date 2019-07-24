#pragma once

#include "ImageAnimation.h"
#include "ImageDescription.h"

#include <unordered_map>


struct ImageAnnotation
{
  ImageDescription description;
  std::unordered_map<std::string, ImageAnimation> animations;
};
