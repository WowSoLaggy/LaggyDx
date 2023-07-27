#include "stdafx.h"
#include "ISpriteShader.h"

#include "SpriteShader.h"


namespace Dx
{
  std::unique_ptr<ISpriteShader> ISpriteShader::create()
  {
    return std::make_unique<SpriteShader>();
  }

} // ns Dx
