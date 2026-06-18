#include "stdafx.h"
#include "ITerrainShader.h"

#include "TerrainShader.h"


namespace Dx
{
  std::shared_ptr<ITerrainShader> ITerrainShader::create(const ICamera3& i_camera)
  {
    return std::make_shared<TerrainShader>(i_camera);
  }

} // ns Dx
