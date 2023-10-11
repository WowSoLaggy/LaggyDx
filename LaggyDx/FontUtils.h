#pragma once

#include <LaggySdk/Rect.h>


namespace Dx
{
  class FontUtils
  {
  public:
    static Sdk::RectI getStringRect(const fs::path& i_fontName, const std::string& i_string);

  private:
    FontUtils() = delete;
  };

} // ns Dx
