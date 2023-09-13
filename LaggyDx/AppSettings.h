#pragma once

#include <LaggySdk/Common.h>


namespace Dx
{
  struct AppSettings
  {
    int screenWidth = 1600;
    int screenHeight = 900;

    int refreshRate = 60;

    std::string applicationName = "DxApp";
    fs::path dataFolder = "Data";
    fs::path assetsFolder = dataFolder / "Assets";

    bool debugMode = false;
  };

} // ns Dx
