#include "stdafx.h"
#include "ISkydomeShader.h"

#include "SkydomeShader.h"


namespace Dx
{
  std::unique_ptr<ISkydomeShader> ISkydomeShader::create(const ICamera& i_camera)
  {
    return std::make_unique<SkydomeShader>(i_camera);
  }

} // ns Dx
