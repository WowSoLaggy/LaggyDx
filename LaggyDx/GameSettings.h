#pragma once

#include <string>


namespace Dx
{
  struct GameSettings
  {
    int screenWidth = 1600;
    int screenHeight = 900;

    int refreshRate = 60;

    std::string applicationName = "DxGame";
    fs::path assetsFolder = "Data/Assets/";

    bool debugMode = false;
  };

} // ns Dx
