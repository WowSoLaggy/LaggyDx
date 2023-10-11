#include "stdafx.h"
#include "FontUtils.h"

#include "App.h"
#include "IFontResource.h"


namespace Dx
{
  Sdk::RectI FontUtils::getStringRect(const fs::path& i_fontName, const std::string& i_string)
  {
    const auto& font = App::get().getResourceController().getFont(i_fontName);
    return font.getStringRect(i_string);
  }

} // ns Dx
