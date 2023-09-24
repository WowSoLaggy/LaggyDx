#include "stdafx.h"
#include "TextureUtils.h"

#include "App.h"


namespace Dx
{
  const Dx::ITexture& TextureUtils::getTexture(const fs::path& i_name)
  {
    const auto& rc = Dx::App::get().getResourceController();
    return rc.getTexture(i_name);
  }

} // ns Dx
