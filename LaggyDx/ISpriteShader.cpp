#include "stdafx.h"
#include "ISpriteShader.h"

#include "SpriteShader.h"


namespace Dx
{
  std::unique_ptr<ISpriteShader> ISpriteShader::create(const ICamera2* i_camera)
  {
    return std::make_unique<SpriteShader>(i_camera);
  }

} // ns Dx
