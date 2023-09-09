#pragma once

#include "IMesh.h"


namespace Dx
{
  class DefaultMeshes
  {
  public:
    static std::unique_ptr<IMesh> rectangle(float i_width, float i_height);

  private:
    DefaultMeshes() = delete;
  };

} // Dx
