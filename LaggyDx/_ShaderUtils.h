#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  void compileShader(
    const fs::path& i_fileName,
    ID3D10Blob** i_buffer,
    const char* i_entryPoint,
    const char* i_target);

} // ns Dx
